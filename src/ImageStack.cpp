#include "ImageStack.h"

#include <QDebug>
#include <QFileInfo>

#include <FreeImage.h>

ImageStack::ImageStack(const QSize& size /*= QSize()*/) :
	_size(size),
	_imageFilePaths()
{
}

QSize ImageStack::size() const
{
	return _size;
}

QStringList ImageStack::imageFilePaths() const
{
	return _imageFilePaths;
}

int ImageStack::noDimensions() const
{
	return noImages();
}

int ImageStack::noImages() const
{
	return _imageFilePaths.size();
}

int ImageStack::noPixels() const
{
	return _size.width() * _size.height();
}

void ImageStack::add(const QString & imageFilePath)
{
	_imageFilePaths.append(imageFilePath);
}

void ImageStack::load()
{
	emit beginLoad(this);

	std::vector<float> pointsData;

	// qDebug() << _imageFilePaths;

	pointsData.resize(noImages() * noPixels());

	qDebug() << pointsData.size();

	foreach(const QString &imageFilePath, _imageFilePaths) {
		const auto imageIndex	= _imageFilePaths.indexOf(imageFilePath);
		const auto done			= imageIndex + 1;
		const auto percentage	= 100.0f * (done / (float)noImages());

		loadImage(imageFilePath, imageIndex, pointsData);

		emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages()), QString::number(percentage, 'f', 1)));
	}

	// qDebug() << pointsData;

	emit message(QString("%1 image(s) loaded").arg(noImages()));

	emit endLoad(this, pointsData);
}

void ImageStack::loadImage(const QString & imageFilePath, const int& imageIndex, std::vector<float>& pointsData)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* image = FreeImage_ConvertToGreyscale(FreeImage_Load(format, imageFilePath.toUtf8()));

	unsigned x, y;

	const auto image_type	= FreeImage_GetImageType(image);
	const auto imageWidth	= FreeImage_GetWidth(image);
	const auto imageHeight	= FreeImage_GetHeight(image);

	// qDebug() << image_type << pointsData.size();
	
	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (y = 0; y < imageHeight; y++) {
					BYTE *bits = (BYTE *)FreeImage_GetScanLine(image, y);
					for (x = 0; x < imageWidth; x++) {
						const auto pixelId = y * imageWidth + x;
						const auto pointId = (pixelId * noDimensions()) + imageIndex;

						pointsData[pointId] = (float)bits[x];
					}
				}
			}

			break;
	}
}