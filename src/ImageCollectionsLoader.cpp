#include "ImageCollectionsLoader.h"
#include "ImageScanner.h"
#include "FreeImage.h"

#include <QDebug>
#include <QFileInfo>

ImageCollectionsLoader::ImageCollectionsLoader(const ImageCollectionType& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(imageCollectionTypeName(type))),
	_type(type),
	_datasetName(),
	_subsampleImageSettings(&_settings)
{
}

ImageCollectionType ImageCollectionsLoader::type() const
{
	return _type;
}

SubsampleImageSettings & ImageCollectionsLoader::subsampleImageSettings()
{
	return _subsampleImageSettings;
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

	Images images(_type);

	images.setName(_datasetName);

	switch (_type)
	{
		case ImageCollectionType::Sequence:
		{
			ImageCollection sequence = imageCollections.map().first();

			images.setSize(sequence.imageSize());

			if (_subsampleImageSettings.enabled())
				images.setSize(sequence.imageSize() * (_subsampleImageSettings.ratio() / 100.f));

			images.setImageFilePaths(sequence.imageFilePaths());

			foreach(const QString& imageFilePath, images.imageFilePaths()) {
				const auto imageIndex = images.imageFilePaths().indexOf(imageFilePath);

				loadImage(imageFilePath, images);

				const auto done			= images.imageFilePaths().indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(images.noImages()));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(images.noImages()), QString::number(percentage, 'f', 1)));
			}

			emit message(QString("%1 image(s) loaded").arg(images.noImages()));
			
			break;
		}

		case ImageCollectionType::Stack:
		{
			ImageCollection stack = imageCollections.map().first();

			images.setSize(stack.imageSize());

			if (_subsampleImageSettings.enabled())
				images.setSize(stack.imageSize() * (_subsampleImageSettings.ratio() / 100.f));

			images.setImageFilePaths(stack.imageFilePaths());
			images.setDimensionNames(images.imageFilePaths());

			foreach(const QString& imageFilePath, images.imageFilePaths()) {
				const auto imageIndex = images.imageFilePaths().indexOf(imageFilePath);

				loadImage(imageFilePath, images);

				const auto done			= images.imageFilePaths().indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(images.noImages()));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(images.noImages()), QString::number(percentage, 'f', 1)));
			}

			emit message(QString("%1 image(s) loaded").arg(images.noImages()));
			
			break;
		}

		case ImageCollectionType::MultiPartSequence:
		{
			/*
			auto imageFilePaths = QStringList();

			imagePointDataSet.setNoDimensions(imageCollections.map().first().noDimensions());

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath = imageCollection.imageFilePaths().first();

				imageFilePaths << imageFilePath;

				auto imageSize = imageCollection.imageSize();

				if (_subsampleImageSettings.enabled())
					imageSize = imageCollection.imageSize() * (_subsampleImageSettings.ratio() / 100.f);

				imagePointDataSet.setImageSize(imageFilePath, imageSize);
			}

			const auto noImages = imageFilePaths.size();

			imagePointDataSet.setImageFilePaths(imageFilePaths);
			
			auto dimensionIds = std::vector<int>();

			dimensionIds.resize(imagePointDataSet.noDimensions());
			std::iota(std::begin(dimensionIds), std::end(dimensionIds), 0);

			auto dimensionNames = QStringList();

			for (int dimensionId : dimensionIds)
			{
				dimensionNames << QString("dim_%1").arg(dimensionId);
			}

			imagePointDataSet.setDimensionNames(dimensionNames);
			
			auto& pointsData = imagePointDataSet.create();

			auto noPointsPerDimension = imagePointDataSet.noPointsPerDimension();

			int imageOffset	= 0;
			int done		= 0;

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath	= imageCollection.imageFilePaths().first();
				const auto imageIndex		= imageFilePaths.indexOf(imageFilePath);
				const auto imageSize		= imagePointDataSet.imageSize(imageFilePath);
				
				auto* multiBitmap = freeImageOpenMultiBitmap(imageFilePath);

				if (multiBitmap != nullptr) {
					const auto noPages = FreeImage_GetPageCount(multiBitmap);

					for (int pageIndex = 0; pageIndex < noPages; pageIndex++) {
						const auto pointIndexMapper = [imageOffset, noPointsPerDimension, pageIndex, imageSize](int x, int y) {
							const auto localPixelIndex = y * imageSize.width() + x;
							return (pageIndex * noPointsPerDimension) + imageOffset + localPixelIndex;
						};

						auto* pageBitmap = FreeImage_LockPage(multiBitmap, pageIndex);

						if (pageBitmap != nullptr) {
							//qDebug() << pageIndex;

							loadBitmap(pageBitmap, imageSize, pointIndexMapper, pointsData);

							FreeImage_UnlockPage(multiBitmap, pageBitmap, false);
						}
					}

					imageOffset += imageSize.width() * imageSize.height();

					done++;

					FreeImage_CloseMultiBitmap(multiBitmap);

					//qDebug() << pointsData;

					const auto percentage = 100.0f * (done / static_cast<float>(noImages));

					emit message(QString("Loaded %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages), QString::number(percentage, 'f', 1)));
				}
			}
			*/
			break;
		}

		default:
			break;
	}

	emit endLoad(images);
}

void ImageCollectionsLoader::loadBitmap(fi::FIBITMAP* bitmap, const QSize& imageSize, Image& image)
{
	assert(bitmap != nullptr);
	
	const auto width	= fi::FreeImage_GetWidth(bitmap);
	const auto height	= fi::FreeImage_GetHeight(bitmap);
	const auto rescale	= QSize(width, height) != imageSize;
	
	auto* scaledBitmap = rescale ? fi::FreeImage_Rescale(bitmap, imageSize.width(), imageSize.height(), static_cast<fi::FREE_IMAGE_FILTER>(_subsampleImageSettings.filter())) : bitmap;
	
	const auto scaledBitmapImageType = fi::FreeImage_GetImageType(bitmap);

	if (scaledBitmap) {
		switch (scaledBitmapImageType) {
			case fi::FIT_BITMAP:
			{
				auto* greyscaleBitmap = fi::FreeImage_ConvertToGreyscale(scaledBitmap);

				if (greyscaleBitmap) {
					for (std::int32_t y = 0; y < imageSize.height(); y++) {
						const fi::BYTE *const bits = fi::FreeImage_GetScanLine(greyscaleBitmap, y);

						for (std::int32_t x = 0; x < imageSize.width(); x++) {
							const auto pixel = static_cast<std::uint16_t>(bits[x]);
							image.setPixel(x, y, &pixel);
						}
					}
				}

				break;
			}
			
			case fi::FIT_UINT16:
			{
				for (std::int32_t y = 0; y < imageSize.height(); y++) {
					unsigned short* bits = (unsigned short*)fi::FreeImage_GetScanLine(scaledBitmap, y);

					for (std::int32_t x = 0; x < imageSize.width(); x++) {
						const auto pixel = bits[x];
						image.setPixel(x, y, &pixel);
					}
				}

				break;
			}
		}

		if (rescale)
			fi::FreeImage_Unload(scaledBitmap);
	}
}

void ImageCollectionsLoader::loadImage(const QString &imageFilePath, Images& images)
{
	const auto format = fi::FreeImage_GetFileType(imageFilePath.toUtf8(), 0);
	
	auto* bitmap = FreeImage::freeImageLoad(imageFilePath);
	
	if (bitmap) {
		loadBitmap(bitmap, images.size(), images.add(1, imageFilePath));

		fi::FreeImage_Unload(bitmap);
	}
}