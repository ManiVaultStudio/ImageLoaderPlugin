#include "ImageCollection.h"

#include <QDebug>

ImageCollection::ImageCollection() :
	_imageSize(),
	_imageFilePaths(),
	_noDimensions(0)
{
}

ImageCollection::ImageCollection(const QSize& imageSize) :
	_imageSize(imageSize),
	_imageFilePaths(),
	_noDimensions(0)
{
}

int ImageCollection::noImages() const
{
	return _imageFilePaths.size();
}

QSize ImageCollection::imageSize() const
{
	return _imageSize;
}

void ImageCollection::setImageSize(const QSize& imageSize)
{
	_imageSize = imageSize;
}

QStringList ImageCollection::imageFilePaths() const
{
	return _imageFilePaths;
}

void ImageCollection::add(const QString& imageFilePath)
{
	_imageFilePaths = _imageFilePaths << imageFilePath;
}

int ImageCollection::noDimensions() const
{
	return _noDimensions;
}

void ImageCollection::setNoDimensions(const int& noDimensions)
{
	_noDimensions = noDimensions;
}

QDebug operator<<(QDebug dbg, const ImageCollection& imageCollection)
{
	const auto imageSize = imageCollection.imageSize();

	dbg << QString("%1 images at %2x%3").arg(QString::number(imageCollection.imageFilePaths().size()), QString::number(imageSize.width()), QString::number(imageSize.height()));

	return dbg;
}