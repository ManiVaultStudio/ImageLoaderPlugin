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

			if (_subsampleImageSettings.enabled()) {
				sequence.setImageSize(sequence.imageSize() * (_subsampleImageSettings.ratio() / 100.f));
			}

			const auto imageFilePaths	= sequence.imageFilePaths();
			const auto noImages			= sequence.noImages();
			const auto imageSize		= sequence.imageSize();
			const auto noDimensions		= imageSize.width() * imageSize.height();
			const auto noPoints			= noImages * noDimensions;
			const auto total			= noImages;

			imageDataSet.setNoImages(noImages);
			imageDataSet.setImageSize(imageSize);
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

			emit message(QString("%1 image(s) loaded").arg(total));

			break;
		}

		case ImageCollectionType::Stack:
		{
			ImageCollection stack = imageCollections.map().first();

			if (_subsampleImageSettings.enabled()) {
				stack.setImageSize(stack.imageSize() * (_subsampleImageSettings.ratio() / 100.f));
			}

			const auto imageFilePaths	= stack.imageFilePaths();
			const auto noImages			= stack.noImages();
			const auto imageSize		= stack.imageSize();
			const auto noPixels			= imageSize.width() * imageSize.height();
			const auto noDimensions		= noImages;
			const auto noPoints			= noImages * noPixels;
			const auto total			= noImages;

			imageDataSet.setNoImages(noImages);
			imageDataSet.setImageSize(imageSize);
			imageDataSet.setNoDimensions(noImages);

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

			emit message(QString("%1 image(s) loaded").arg(total));

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

	auto* image = FreeImage_ConvertToGreyscale(freeImageLoad(imageFilePath));

	const auto image_type	= FreeImage_GetImageType(image);
	const auto noPixels		= imageSize.width() * imageSize.height();

	//qDebug() << image_type;

	const auto width	= FreeImage_GetWidth(image);
	const auto height	= FreeImage_GetHeight(image);

	if (QSize(width, height) != imageSize) {
		image = FreeImage_Rescale(image, imageSize.width(), imageSize.height(), static_cast<FREE_IMAGE_FILTER>(_subsampleImageSettings.filter()));
	}

	switch (image_type) {
		case FIT_BITMAP: {
			if (FreeImage_GetBPP(image) == 8) {
				for (unsigned y = 0; y < imageSize.height(); y++) {
					const BYTE *const bits = FreeImage_GetScanLine(image, y);
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
}

/*
void ImageSequenceLoader::load()
{
	emit beginLoad();

	emit message(QString("Loading %1 images").arg(noImages()));

	FloatVector pointsData;

	pointsData.clear();

	pointsData.reserve(noImages() * noDimensions());

	const auto total = noImages();

	foreach(const QString &imageFilePath, _imageFilePaths) {
		loadImage(imageFilePath, _imageFilePaths.indexOf(imageFilePath), pointsData);

		const auto done = _imageFilePaths.indexOf(imageFilePath) + 1;
		const auto percentage = 100.0f * (done / static_cast<float>(total));

		emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
	}

	emit message(QString("%1 image(s) loaded").arg(total));

	emit endLoad(pointsData);

}


*/

/*
void ImageStackLoader::load()
{
	emit beginLoad();

	std::vector<float> pointsData;

	// qDebug() << _imageFilePaths;

	pointsData.resize(noImages() * noPixels());

	qDebug() << pointsData.size();

	foreach(const QString &imageFilePath, _imageFilePaths) {
		const auto imageIndex	= _imageFilePaths.indexOf(imageFilePath);
		const auto done			= imageIndex + 1;
		const auto percentage	= 100.0f * (done / static_cast<float>(noImages()));

		loadImage(imageFilePath, imageIndex, pointsData);

		emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages()), QString::number(percentage, 'f', 1)));
	}

	// qDebug() << pointsData;

	emit message(QString("%1 image(s) loaded").arg(noImages()));

	emit endLoad(this, pointsData);
}

void ImageStackLoader::loadImage(const QString & imageFilePath, const int& imageIndex, std::vector<float>& pointsData)
{
	/*
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* image = FreeImage_ConvertToGreyscale(freeImageLoad(imageFilePath));

	const auto image_type	= FreeImage_GetImageType(image);
	const auto imageWidth	= FreeImage_GetWidth(image);
	const auto imageHeight	= FreeImage_GetHeight(image);

	// qDebug() << image_type << pointsData.size();

	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (unsigned y = 0; y < imageHeight; y++) {
					const BYTE *const bits = FreeImage_GetScanLine(image, y);
					for (unsigned x = 0; x < imageWidth; x++) {
						const auto pixelId = y * imageWidth + x;
						const auto pointId = (pixelId * noDimensions()) + imageIndex;

						pointsData[pointId] = static_cast<float>(bits[x]);
					}
				}
			}

			break;
	}
}
*/