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

				loadImage(imageFilePath, images);

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

				loadImage(imageFilePath, images);

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
							loadBitmap(pageBitmap, images.size(), images.add(1, imageFilePath));

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

void ImageCollectionsLoader::loadBitmap(fi::FIBITMAP* bitmap, const QSize& imageSize, Image& image)
{
	assert(bitmap != nullptr);
	
	const auto width	= fi::FreeImage_GetWidth(bitmap);
	const auto height	= fi::FreeImage_GetHeight(bitmap);
	const auto rescale	= QSize(width, height) != imageSize;
	
	auto* scaledBitmap = rescale ? fi::FreeImage_Rescale(bitmap, imageSize.width(), imageSize.height(), static_cast<fi::FREE_IMAGE_FILTER>(_subsampleSettings.filter())) : bitmap;
	
	const auto scaledBitmapImageType = fi::FreeImage_GetImageType(bitmap);

	auto pixel = std::vector<std::uint16_t>();

	pixel.resize(image.noComponents());

	if (scaledBitmap) {
		switch (scaledBitmapImageType) {
			case fi::FIT_BITMAP:
			{
				auto* greyscaleBitmap = fi::FreeImage_ConvertToGreyscale(scaledBitmap);

				if (greyscaleBitmap) {
					for (std::int32_t y = 0; y < imageSize.height(); y++) {
						const fi::BYTE *const bits = fi::FreeImage_GetScanLine(greyscaleBitmap, y);

						for (std::int32_t x = 0; x < imageSize.width(); x++) {
							std::fill(pixel.begin(), pixel.end(), static_cast<std::uint16_t>(bits[x]));

							image.setPixel(x, y, pixel.data());
						}
					}
				}

				break;
			}
			
			case fi::FIT_UINT16:
			{
				for (std::int32_t y = 0; y < imageSize.height(); y++) {
					const fi::BYTE *const scanline = fi::FreeImage_GetScanLine(scaledBitmap, y);

					for (std::int32_t x = 0; x < imageSize.width(); x++) {
						std::fill(pixel.begin(), pixel.end(), static_cast<std::uint16_t>(scanline[x]));

						image.setPixel(x, y, pixel.data());
					}
				}
				
				break;
			}
		}

		if (rescale)
			fi::FreeImage_Unload(scaledBitmap);
	}

	qDebug() << image;

	image.save("kjhg.jpg");
}

void ImageCollectionsLoader::loadImage(const QString &imageFilePath, Images& images)
{
	const auto format = fi::FreeImage_GetFileType(imageFilePath.toUtf8(), 0);
	
	auto* bitmap = FreeImage::freeImageLoad(imageFilePath);
	
	if (bitmap) {
		loadBitmap(bitmap, images.size(), images.add(4, imageFilePath));

		fi::FreeImage_Unload(bitmap);
	}
}