#include "ImageCollectionsLoader.h"
#include "ImageScanner.h"
#include "FreeImage.h"

#include <QDebug>
#include <QFileInfo>

ImageCollectionsLoader::ImageCollectionsLoader(const ImageCollectionType& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(imageCollectionTypeName(type))),
	_type(type),
	_datasetName(),
	_subsampleSettings(&_settings),
	_colorSettings(&_settings)
{
}

ImageCollectionType ImageCollectionsLoader::type() const
{
	return _type;
}

SubsampleSettings & ImageCollectionsLoader::subsampleImageSettings()
{
	return _subsampleSettings;
}

ColorSettings& ImageCollectionsLoader::colorSettings()
{
	return _colorSettings;
}

QVariant ImageCollectionsLoader::setting(const QString& name, const QVariant& defaultValue) const
{
	return _settings.value(name, defaultValue).toString();
}

void ImageCollectionsLoader::setSetting(const QString& name, const QVariant& value)
{
	_settings.setValue(name, value);
}

QString ImageCollectionsLoader::datasetName() const
{
	return _datasetName;
}

void ImageCollectionsLoader::setDatasetName(const QString& datasetName)
{
	if (datasetName == _datasetName)
		return;

	_datasetName = datasetName;

	emit datasetNameChanged(_datasetName);
}

void ImageCollectionsLoader::load(const ImageCollections& scannedImageCollections)
{
	emit beginLoad();

	ImageCollections imageCollections = scannedImageCollections;

	switch (_type)
	{
		case ImageCollectionType::Sequence:
		{
			Images images(_type);

			images.setName(_datasetName);

			ImageCollection sequence = imageCollections.map().first();

			images.setSize(sequence.imageSize());

			if (_subsampleSettings.enabled())
				images.setSize(sequence.imageSize() * (_subsampleSettings.ratio() / 100.f));

			images.setImageFilePaths(sequence.imageFilePaths());

			foreach(const QString& imageFilePath, images.imageFilePaths()) {
				const auto imageIndex = images.imageFilePaths().indexOf(imageFilePath);

				loadBitmap(imageFilePath, images);

				const auto done			= images.imageFilePaths().indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(images.noImages()));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(images.noImages()), QString::number(percentage, 'f', 1)));
			}

			emit endLoad(images);
			emit message(QString("%1 image(s) loaded").arg(images.noImages()));
			
			break;
		}

		case ImageCollectionType::Stack:
		{
			Images images(_type);

			images.setName(_datasetName);

			ImageCollection stack = imageCollections.map().first();

			images.setSize(stack.imageSize());

			if (_subsampleSettings.enabled())
				images.setSize(stack.imageSize() * (_subsampleSettings.ratio() / 100.f));

			images.setImageFilePaths(stack.imageFilePaths());
			images.setDimensionNames(images.imageFilePaths());

			foreach(const QString& imageFilePath, images.imageFilePaths()) {
				const auto imageIndex = images.imageFilePaths().indexOf(imageFilePath);

				loadBitmap(imageFilePath, images);

				const auto done			= images.imageFilePaths().indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(images.noImages()));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(images.noImages()), QString::number(percentage, 'f', 1)));
			}

			emit endLoad(images);
			emit message(QString("%1 image(s) loaded").arg(images.noImages()));
			
			break;
		}

		case ImageCollectionType::MultiPartSequence:
		{
			std::uint32_t done = 0;
			
			const auto noImages = imageCollections.map().size();

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath = imageCollection.imageFilePaths().first();

				Images images(_type);

				images.setName(QString("%1-%2").arg(_datasetName, imageFilePath));

				images.setSize(imageCollection.imageSize());

				if (_subsampleSettings.enabled())
					images.setSize(imageCollection.imageSize() * (_subsampleSettings.ratio() / 100.f));

				auto* multiBitmap = FreeImage::freeImageOpenMultiBitmap(imageFilePath);

				if (multiBitmap != nullptr) {
					const auto noPages = FreeImage_GetPageCount(multiBitmap);

					for (int pageIndex = 0; pageIndex < noPages; pageIndex++) {
						auto* pageBitmap = FreeImage_LockPage(multiBitmap, pageIndex);

						if (pageBitmap != nullptr) {
							//loadBitmap(pageBitmap, images.size(), images.add(1, imageFilePath));

							FreeImage_UnlockPage(multiBitmap, pageBitmap, false);
						}
					}

					done++;

					FreeImage_CloseMultiBitmap(multiBitmap);
				}
				
				const auto percentage = 100.0f * (done / static_cast<float>(noImages));

				emit endLoad(images);
				emit message(QString("Loaded %1 (%2%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(percentage)));
			}
			
			emit message(QString("Loaded %1 multipart images").arg(QString::number(noImages)));

			break;
		}

		default:
			break;
	}
}

void ImageCollectionsLoader::loadBitmap(const QString &imageFilePath, Images& images)
{
	const auto format = fi::FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* bitmap = FreeImage::freeImageLoad(imageFilePath);

	if (bitmap) {
		const auto width	= fi::FreeImage_GetWidth(bitmap);
		const auto height	= fi::FreeImage_GetHeight(bitmap);
		const auto rescale	= QSize(width, height) != images.size();

		auto* scaledBitmap = rescale ? fi::FreeImage_Rescale(bitmap, images.size().width(), images.size().height(), static_cast<fi::FREE_IMAGE_FILTER>(_subsampleSettings.filter())) : bitmap;

		if (scaledBitmap) {
			const auto imageType = fi::FreeImage_GetImageType(scaledBitmap);

			auto noComponents = 1;

			switch (imageType)
			{
				case fi::FIT_BITMAP:
					noComponents = 3;
					break;

				case fi::FIT_UINT16:
				case fi::FIT_INT16:
				case fi::FIT_UINT32:
				case fi::FIT_INT32:
				case fi::FIT_FLOAT:
				case fi::FIT_DOUBLE:
				case fi::FIT_COMPLEX:
					noComponents = 1;
					break;
				
				case fi::FIT_RGB16:
				case fi::FIT_RGBF:
					noComponents = 3;
					break;

				case fi::FIT_RGBA16:
				case fi::FIT_RGBAF:
					noComponents = 4;
					break;

				default:
					break;
			}

			if (_colorSettings.convertToGrayscale())
				noComponents = 1;

			auto& image = images.add(noComponents, imageFilePath);

			auto pixel = std::vector<std::uint16_t>();

			pixel.resize(image.noComponents());

			if (scaledBitmap) {
				const auto bpp = fi::FreeImage_GetBPP(scaledBitmap);

				switch (imageType) {
					case fi::FIT_BITMAP:
					{
						for (std::int32_t y = 0; y < images.size().height(); y++)
						{
							fi::BYTE* scanLine = (fi::BYTE*)fi::FreeImage_GetScanLine(scaledBitmap, y);

							for (std::int32_t x = 0; x < images.size().width(); x++)
							{
								switch (bpp)
								{
									case 8:
									{
										switch (image.noComponents())
										{
											case 1:
											{
												pixel[0] = scanLine[x];
												break;
											}

											case 3:
											{
												pixel[0] = scanLine[x];
												pixel[1] = scanLine[x];
												pixel[2] = scanLine[x];
												break;
											}

											default:
												break;
										}

										break;
									}

									case 24:
									{
										switch (image.noComponents())
										{
											case 1:
											{
												pixel[0] = scanLine[x * 3 + FI_RGBA_RED];
												break;
											}

											case 3:
											{
												pixel[0] = scanLine[x * image.noComponents() + FI_RGBA_RED];
												pixel[1] = scanLine[x * image.noComponents() + FI_RGBA_GREEN];
												pixel[2] = scanLine[x * image.noComponents() + FI_RGBA_BLUE];
												break;
											}

											default:
												break;
										}
										
										break;
									}

									default:
										break;
								}

								image.setPixel(x, y, pixel.data());
							}
						}

						break;
					}

					case fi::FIT_UINT16:
					{
						for (std::int32_t y = 0; y < images.size().height(); y++) {
							std::uint16_t* scanLine = (std::uint16_t*)fi::FreeImage_GetScanLine(scaledBitmap, y);

							for (std::int32_t x = 0; x < images.size().width(); x++) {
								pixel[0] = scanLine[x];
								image.setPixel(x, y, pixel.data());
							}
						}

						break;
					}
				}

				if (rescale)
					fi::FreeImage_Unload(scaledBitmap);
			}

			fi::FreeImage_Unload(bitmap);
		}
	}
}