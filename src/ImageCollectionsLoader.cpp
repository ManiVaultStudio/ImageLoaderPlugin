#include "ImageCollectionsLoader.h"
#include "ImageUtilities.h"
#include "ImageScanner.h"

#include <QDebug>

ImageCollectionsLoader::ImageCollectionsLoader(const ImageCollectionType& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(imageCollectionTypeName(type))),
	_type(type),
	_imageFilePaths(),
	_subsampleImageSettings(&_settings)
{
}

ImageCollectionType ImageCollectionsLoader::type() const
{
	return _type;
}

QStringList ImageCollectionsLoader::imageFilePaths() const
{
	return _imageFilePaths;
}

int ImageCollectionsLoader::noImages() const
{
	return _imageFilePaths.size();
}

SubsampleImageSettings & ImageCollectionsLoader::subsampleImageSettings()
{
	return _subsampleImageSettings;
}

void ImageCollectionsLoader::reset()
{
	_imageFilePaths.clear();
}

QVariant ImageCollectionsLoader::setting(const QString& name, const QVariant& defaultValue) const
{
	return _settings.value(name, defaultValue).toString();
}

void ImageCollectionsLoader::setSetting(const QString& name, const QVariant& value)
{
	_settings.setValue(name, value);
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

void ImageSequenceLoader::loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto *image = FreeImage_ConvertToGreyscale(freeImageLoad(imageFilePath));

	const auto image_type = FreeImage_GetImageType(image);

	// qDebug() << image_type;

	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (unsigned y = 0; y <  FreeImage_GetHeight(image); y++) {
					const BYTE *const bits = FreeImage_GetScanLine(image, y);
					for (unsigned x = 0; x <  FreeImage_GetWidth(image); x++) {
						pointsData.push_back(static_cast<float>(bits[x]));
					}
				}
			}
			break;
	}
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