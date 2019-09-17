#include "ImageCollectionsLoader.h"
#include "ImageUtilities.h"
#include "ImageScanner.h"

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

	auto imageDataSet = ImageDataSet(_type);

	imageDataSet.setName(_datasetName);


	switch (_type)
	{
		case ImageCollectionType::Sequence:
		{
			ImageCollection sequence = imageCollections.map().first();

			if (_subsampleImageSettings.enabled())
				imageDataSet.setImageSize("0", sequence.imageSize() * (_subsampleImageSettings.ratio() / 100.f));
			else
				imageDataSet.setImageSize("0", sequence.imageSize());

			const auto imageFilePaths	= sequence.imageFilePaths();
			const auto noImages			= sequence.noImages();
			const auto imageSize		= imageDataSet.imageSize("0");
			const auto noPixels			= imageSize.width() * imageSize.height();
			const auto noDimensions		= noPixels;
			const auto noPoints			= noImages * noDimensions;
			const auto total			= noImages;

			imageDataSet.setNoDimensions(noDimensions);

			emit message(QString("Loading %1 images").arg(QString::number(noImages)));

			auto& pointsData = imageDataSet.pointsData();

			pointsData.clear();
			pointsData.resize(noPoints);

			auto imageFileNames = QStringList();

			foreach(const QString& imageFilePath, imageFilePaths) {
				imageFileNames << QFileInfo(imageFilePath).fileName();

				const auto imageIndex = imageFilePaths.indexOf(imageFilePath);

				const auto pointIndexMapper = [noPixels, imageIndex, imageSize](int x, int y) -> int {
					const auto localPixelIndex = y * imageSize.width() + x;
					return imageIndex * noPixels + localPixelIndex;
				};

				loadImage(imageFilePath, imageSize, pointIndexMapper, pointsData);

				const auto done			= imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(total));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
			}

			imageDataSet.setImageFileNames(imageFileNames);

			emit message(QString("%1 image(s) loaded").arg(total));

			break;
		}

		case ImageCollectionType::Stack:
		{
			ImageCollection stack = imageCollections.map().first();

			if (_subsampleImageSettings.enabled())
				imageDataSet.setImageSize("0", stack.imageSize() * (_subsampleImageSettings.ratio() / 100.f));
			else
				imageDataSet.setImageSize("0", stack.imageSize());

			const auto imageFilePaths	= stack.imageFilePaths();
			const auto noImages			= stack.noImages();
			const auto imageSize		= imageDataSet.imageSize("0");
			const auto noPixels			= imageSize.width() * imageSize.height();
			const auto noDimensions		= noImages;
			const auto noPoints			= noImages * noPixels;
			const auto total			= noImages;
			
			auto imageFileNames = QStringList();

			foreach(const QString& imageFilePath, imageFilePaths) {
				imageFileNames << QFileInfo(imageFilePath).fileName();
			}

			imageDataSet.setImageFileNames(imageFileNames);
			imageDataSet.setNoDimensions(noImages);
			imageDataSet.setDimensionNames(imageFileNames);

			auto& pointsData = imageDataSet.pointsData();

			emit message(QString("Loading %1 images").arg(QString::number(noImages)));

			pointsData.clear();
			pointsData.resize(noPoints);

			foreach(const QString& imageFilePath, imageFilePaths) {
				const auto imageIndex = imageFilePaths.indexOf(imageFilePath);

				const auto pointIndexMapper = [noImages, imageIndex, noPixels, imageSize](int x, int y) -> int {
					const auto localPixelIndex = y * imageSize.width() + x;
					return localPixelIndex * noImages + imageIndex;
				};

				loadImage(imageFilePath, imageSize, pointIndexMapper, pointsData);

				const auto done			= imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(total));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
			}

			emit message(QString("%1 image(s) loaded").arg(total));
			
			break;
		}

		case ImageCollectionType::MultiPartSequence:
		{
			auto imageFilePaths = QStringList();
			auto imageFileNames = QStringList();

			int noPoints = 0;

			imageDataSet.setNoDimensions(imageCollections.map().first().noDimensions());

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath = imageCollection.imageFilePaths().first();

				imageFilePaths << imageFilePath;

				imageFileNames << QFileInfo(imageFilePath).fileName();

				auto imageSize = imageCollection.imageSize();

				if (_subsampleImageSettings.enabled())
					imageSize = imageCollection.imageSize() * (_subsampleImageSettings.ratio() / 100.f);

				imageDataSet.setImageSize(imageFilePath, imageSize);

				noPoints += imageSize.width() * imageSize.height() * imageDataSet.noDimensions();
			}

			const auto noImages = imageFilePaths.size();

			imageDataSet.setImageFileNames(imageFileNames);

			auto dimensionNames = QStringList();

			auto& pointsData = imageDataSet.pointsData();

			pointsData.clear();
			pointsData.resize(noPoints);
			
			emit message(QString("Loading %1 images").arg(QString::number(noImages)));
			
			auto noPointsPerDimension = 0;

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath	= imageCollection.imageFilePaths().first();
				const auto imageSize		= imageDataSet.imageSize(imageFilePath);

				noPointsPerDimension += imageSize.width() * imageSize.height();
			}
			
			auto pointIndexOffset	= 0;
			auto done				= 0;

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath	= imageCollection.imageFilePaths().first();
				const auto imageIndex		= imageFilePaths.indexOf(imageFilePath);
				const auto imageSize		= imageDataSet.imageSize(imageFilePath);
				
				auto* multiBitmap = freeImageOpenMultiBitmap(imageFilePath);

				if (multiBitmap != nullptr) {
					const auto noPages = freeImageGetPageCount(imageFilePath);

					for (int pageIndex = 0; pageIndex < noPages; pageIndex++) {
						const auto pointIndexMapper = [pointIndexOffset, noPointsPerDimension, pageIndex, imageSize](int x, int y) -> int {
							const auto localPixelIndex = y * imageSize.width() + x;
							return noPointsPerDimension * pageIndex + localPixelIndex;
						};

						auto* pageBitmap = FreeImage_LockPage(multiBitmap, pageIndex);

						if (pageBitmap != nullptr) {
							loadBitmap(pageBitmap, imageSize, pointIndexMapper, pointsData);

							FreeImage_UnlockPage(multiBitmap, pageBitmap, false);
						}
					}

					pointIndexOffset += imageSize.width() * imageSize.height();

					done++;

					FreeImage_CloseMultiBitmap(multiBitmap);

					const auto percentage = 100.0f * (done / static_cast<float>(noImages));

					emit message(QString("Loaded %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages), QString::number(percentage, 'f', 1)));
				}
			}

			break;
		}

		default:
			break;
	}

	qDebug() << imageDataSet;

	emit endLoad(imageDataSet);
}

template<typename PointIndexMapper>
void ImageCollectionsLoader::loadBitmap(FIBITMAP* bitmap, const QSize& imageSize, const PointIndexMapper& pointIndexMapper, FloatVector& pointsData)
{
	assert(bitmap != nullptr);

	auto* greyscaleBitmap = FreeImage_ConvertToGreyscale(bitmap);

	if (greyscaleBitmap) {
		const auto image_type = FreeImage_GetImageType(greyscaleBitmap);

		const auto width	= FreeImage_GetWidth(greyscaleBitmap);
		const auto height	= FreeImage_GetHeight(greyscaleBitmap);
		const auto rescale	= QSize(width, height) != imageSize;

		auto* rescaledBitmap = rescale ? FreeImage_Rescale(greyscaleBitmap, imageSize.width(), imageSize.height(), static_cast<FREE_IMAGE_FILTER>(_subsampleImageSettings.filter())) : greyscaleBitmap;

		if (rescaledBitmap) {
			for (unsigned y = 0; y < imageSize.height(); y++) {
				const BYTE *const bits = FreeImage_GetScanLine(rescaledBitmap, y);

				for (unsigned x = 0; x < imageSize.width(); x++) {
					pointsData[pointIndexMapper(x, y)] = static_cast<float>(bits[x]);
				}
			}

			FreeImage_Unload(rescaledBitmap);
		}

		if (rescale)
			FreeImage_Unload(greyscaleBitmap);
	}
}

template<typename PointIndexMapper>
void ImageCollectionsLoader::loadImage(const QString& imageFilePath, const QSize& imageSize, const PointIndexMapper& pointIndexMapper, FloatVector& pointsData)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* bitmap = freeImageLoad(imageFilePath);
	
	if (bitmap) {
		loadBitmap(bitmap, imageSize, pointIndexMapper, pointsData);

		FreeImage_Unload(bitmap);
	}
}