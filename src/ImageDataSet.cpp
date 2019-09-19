#include "ImageDataSet.h"

#include <QDebug>

ImageDataSet::ImageDataSet(const ImageCollectionType& type) :
	_type(type),
	_name(),
	_imageFilePaths(),
	_imageSizes(),
	_noDimensions(0),
	_dimensionNames(),
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

QStringList ImageDataSet::imageFilePaths() const
{
	return _imageFilePaths;
}

void ImageDataSet::setImageFilePaths(const QStringList& imageFilePaths)
{
	_imageFilePaths = imageFilePaths;
}

int ImageDataSet::noImages() const
{
	return _imageFilePaths.size();
}

QMap<QString, QVariant> ImageDataSet::imageSizes() const
{
	auto imageSizes = QMap<QString, QVariant>();

	foreach(const QString& imageName, _imageSizes.keys()) {
		imageSizes[imageName] = QVariant(_imageSizes[imageName]);
	}

	return imageSizes;
}

QSize ImageDataSet::imageSize(const QString& imageName) const
{
	if (!_imageSizes.contains(imageName))
		return QSize();

	return _imageSizes[imageName];
}

void ImageDataSet::setImageSize(const QString& imageName, const QSize & imageSize)
{
	_imageSizes[imageName] = imageSize;
}

int ImageDataSet::noDimensions() const
{
	return _noDimensions;
}

void ImageDataSet::setNoDimensions(const int& noDimensions)
{
	_noDimensions = noDimensions;
}

QStringList ImageDataSet::dimensionNames() const
{
	return _dimensionNames;
}

void ImageDataSet::setDimensionNames(const QStringList & dimensionNames)
{
	_dimensionNames = dimensionNames;
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
