#include "ImageStackLoader.h"
#include "ImageUtilities.h"

#include <QDebug>
#include <QDir>

ImageStackLoader::ImageStackLoader() :
	ImageCollectionLoader(ImageCollectionType::Stack)
{
}

QStringList ImageStackLoader::dimensionNames() const
{
	auto dimensionNames = QStringList();
	/*
	foreach(const QString& imageFilePath, _imageFilePaths) {
		dimensionNames << QFileInfo(imageFilePath).fileName();
	}
	*/
	return dimensionNames;
}

int ImageStackLoader::noDimensions() const
{
	return noImages();
}

void ImageStackLoader::add(const QString & imageFilePath)
{
//	_imageFilePaths.append(imageFilePath);
}

void ImageStackLoader::load()
{
	/*
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
	*/
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
	*/
}