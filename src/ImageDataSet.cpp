#include "ImageDataSet.h"

#include <QDebug>

ImageDataSet::ImageDataSet(const ImageCollectionType& type) :
	_type(type),
	_name(),
	_noImages(0),
	_imageSize(),
	_noDimensions(0),
	_pointsData()
{
}

QString ImageDataSet::name() const
{
	return _name;
}

void ImageDataSet::setName(const QString& name)
{
	_name = name;
}

ImageCollectionType ImageDataSet::type() const
{
	return _type;
}

void ImageDataSet::setType(const ImageCollectionType& type)
{
	_type = type;
}

int ImageDataSet::noImages() const
{
	return _noImages;
}

void ImageDataSet::setNoImages(const int& noImages)
{
	_noImages = noImages;
}

QSize ImageDataSet::imageSize() const
{
	return _imageSize;
}

void ImageDataSet::setImageSize(const QSize & imageSize)
{
	_imageSize = imageSize;
}

int ImageDataSet::noDimensions() const
{
	return _noDimensions;
}

void ImageDataSet::setNoDimensions(const int& noDimensions)
{
	_noDimensions = noDimensions;
}

FloatVector& ImageDataSet::pointsData()
{
	return _pointsData;
}

QDebug operator<<(QDebug dbg, ImageDataSet & imageDataSet)
{
	const auto noImages		= QString::number(imageDataSet.noImages());
	const auto noDimensions = QString::number(imageDataSet.noDimensions());
	const auto noPoints		= QString::number(imageDataSet.pointsData().size());

	dbg << QString("Image dataset '%1' of type %2, %3 images, %4 dimensions and %5 points").arg(imageDataSet.name(), imageCollectionTypeName(imageDataSet.type()), noImages, noDimensions, noPoints);

	return dbg;
}
