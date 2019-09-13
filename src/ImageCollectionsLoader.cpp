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
			const auto noDimensions		= imageSize.width() * imageSize.height();
			const auto noPoints			= noImages * noDimensions;
			const auto total			= noImages;

			imageDataSet.setNoImages(noImages);
			imageDataSet.setNoDimensions(noDimensions);

			emit message(QString("Loading %1 images").arg(QString::number(noImages)));

			FloatVector& pointsData = imageDataSet.pointsData();

			pointsData.clear();
			pointsData.resize(noPoints);

			foreach(const QString& imageFilePath, imageFilePaths) {
				loadImage(imageFilePath, imageSize, imageFilePaths.indexOf(imageFilePath), noImages, pointsData);

				const auto done			= imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(total));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
			}

			emit message(QString("%1 greyscaleImage(s) loaded").arg(total));

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
			
			auto dimensionNames = QStringList();

			foreach(const QString& imageFilePath, imageFilePaths) {
				dimensionNames << QFileInfo(imageFilePath).fileName();
			}

			imageDataSet.setNoImages(noImages);
			imageDataSet.setNoDimensions(noImages);
			imageDataSet.setDimensionNames(dimensionNames);

			FloatVector& pointsData = imageDataSet.pointsData();

			emit message(QString("Loading %1 images").arg(QString::number(noImages)));

			pointsData.clear();
			pointsData.resize(noPoints);

			foreach(const QString& imageFilePath, imageFilePaths) {
				loadImage(imageFilePath, imageSize, imageFilePaths.indexOf(imageFilePath), noImages, pointsData);

				const auto done = imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage = 100.0f * (done / static_cast<float>(total));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
			}

			emit message(QString("%1 greyscaleImage(s) loaded").arg(total));

			break;
		}

		case ImageCollectionType::MultiPartSequence:
		{
			auto imageFilePaths = QStringList();

			int noPoints = 0;

			imageDataSet.setNoDimensions(imageCollections.map().first().noDimensions());

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath = imageCollection.imageFilePaths().first();

				imageFilePaths << imageFilePath;

				auto imageSize = imageCollection.imageSize();

				if (_subsampleImageSettings.enabled())
					imageSize = imageCollection.imageSize() * (_subsampleImageSettings.ratio() / 100.f);

				imageDataSet.setImageSize(imageFilePath, imageSize);

				noPoints += imageSize.width() * imageSize.height() * imageDataSet.noDimensions();
			}

			const auto noImages = imageFilePaths.size();

			imageDataSet.setNoImages(noImages);

			auto dimensionNames = QStringList();

			FloatVector& pointsData = imageDataSet.pointsData();

			pointsData.clear();
			pointsData.resize(noPoints);

			emit message(QString("Loading %1 images").arg(QString::number(noImages)));
			
			auto noPointsPerDimension = 0;

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath = imageCollection.imageFilePaths().first();
				const auto imageSize = imageDataSet.imageSize(imageFilePath);
				
				noPointsPerDimension += imageSize.width() * imageSize.height();
			}

			auto pointIndexOffset	= 0;
			auto done				= 0;

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath	= imageCollection.imageFilePaths().first();
				const auto imageSize		= imageDataSet.imageSize(imageFilePath);
				
				loadMultiPartImage(imageFilePath, imageSize, pointIndexOffset, noPointsPerDimension, pointsData);
				
				pointIndexOffset += imageSize.width() * imageSize.height();

				done++;

				const auto percentage = 100.0f * (done / static_cast<float>(noImages));

				emit message(QString("Loaded %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages), QString::number(percentage, 'f', 1)));
			}

			/*
			foreach(const QString& imageFilePath, imageFilePaths) {
				loadImage(imageFilePath, imageSize, imageFilePaths.indexOf(imageFilePath), noImages, pointsData);

				const auto done = imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage = 100.0f * (done / static_cast<float>(total));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
			}
			*/

			// emit message(QString("%1 greyscaleImage(s) loaded").arg(total));

			break;
		}

		default:
			break;
	}

	qDebug() << imageDataSet;

	emit endLoad(imageDataSet);
}

void ImageCollectionsLoader::loadImage(const QString& imageFilePath, const QSize& imageSize, const int& imageIndex, const int& noImages, FloatVector& pointsData)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* originalImage		= freeImageLoad(imageFilePath);
	auto* greyscaleImage	= FreeImage_ConvertToGreyscale(originalImage);

	const auto image_type	= FreeImage_GetImageType(greyscaleImage);
	const auto noPixels		= imageSize.width() * imageSize.height();

	const auto width = FreeImage_GetWidth(greyscaleImage);
	const auto height = FreeImage_GetHeight(greyscaleImage);

	if (QSize(width, height) != imageSize) {
		greyscaleImage = FreeImage_Rescale(greyscaleImage, imageSize.width(), imageSize.height(), static_cast<FREE_IMAGE_FILTER>(_subsampleImageSettings.filter()));
	}

	switch (image_type) {
		case FIT_BITMAP: {
			if (FreeImage_GetBPP(greyscaleImage) == 8) {
				for (unsigned y = 0; y < imageSize.height(); y++) {
					const BYTE *const bits = FreeImage_GetScanLine(greyscaleImage, y);
					for (unsigned x = 0; x < imageSize.width(); x++) {
						const auto localPixelIndex = y * imageSize.width() + x;

						switch (_type)
						{
							case ImageCollectionType::Sequence:
							{
								const auto pointIndex = imageIndex * noPixels + localPixelIndex;
								pointsData[pointIndex] = static_cast<float>(bits[x]);
								break;
							}

							case ImageCollectionType::Stack:
							{
								const auto pointIndex = localPixelIndex * noImages + imageIndex;
								pointsData[pointIndex] = static_cast<float>(bits[x]);
								break;
							}
						}
					}
				}
			}
			break;
		}
	}

	FreeImage_Unload(originalImage);
	FreeImage_Unload(greyscaleImage);
}

void ImageCollectionsLoader::loadMultiPartImage(const QString& imageFilePath, const QSize& imageSize, const int& pointIndexOffset, const int& noPointsPerDimension, FloatVector& pointsData)
{
	/*
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* greyscaleImage = FreeImage_ConvertToGreyscale(freeImageLoad(imageFilePath));

	const auto image_type	= FreeImage_GetImageType(greyscaleImage);
	const auto noPixels		= imageSize.width() * imageSize.height();

	//qDebug() << image_type;

	const auto width	= FreeImage_GetWidth(greyscaleImage);
	const auto height	= FreeImage_GetHeight(greyscaleImage);

	if (QSize(width, height) != imageSize) {
		greyscaleImage = FreeImage_Rescale(greyscaleImage, imageSize.width(), imageSize.height(), static_cast<FREE_IMAGE_FILTER>(_subsampleImageSettings.filter()));
	}

	switch (image_type) {
		case FIT_BITMAP: {
			if (FreeImage_GetBPP(greyscaleImage) == 8) {
				for (unsigned y = 0; y < imageSize.height(); y++) {
					const BYTE *const bits = FreeImage_GetScanLine(greyscaleImage, y);
					for (unsigned x = 0; x < imageSize.width(); x++) {
						const auto localPixelIndex = y * imageSize.width() + x;

						switch (_type)
						{
							case ImageCollectionType::Sequence:
							{
								const auto pointIndex = imageIndex * noPixels + localPixelIndex;
								pointsData[pointIndex] = static_cast<float>(bits[x]);
								break;
							}

							case ImageCollectionType::Stack:
							{
								const auto pointIndex = localPixelIndex * noImages + imageIndex;
								pointsData[pointIndex] = static_cast<float>(bits[x]);
								break;
							}
						}
					}
				}
			}
			break;
		}
	}
	*/
}