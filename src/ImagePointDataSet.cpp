#include "ImagePointDataSet.h"

#include <QDebug>

ImagePointDataSet::ImagePointDataSet(const ImageCollectionType& type) :
	_type(type),
	_name(),
	_imageFilePaths(),
	_imageSizes(),
	_noDimensions(0),
	_dimensionNames(),
	_pointsData()
{
}

QString ImagePointDataSet::name() const
{
	return _name;
}

void ImagePointDataSet::setName(const QString& name)
{
	_name = name;
}

ImageCollectionType ImagePointDataSet::type() const
{
	return _type;
}

void ImagePointDataSet::setType(const ImageCollectionType& type)
{
	_type = type;
}

QStringList ImagePointDataSet::imageFilePaths() const
{
	return _imageFilePaths;
}

void ImagePointDataSet::setImageFilePaths(const QStringList& imageFilePaths)
{
	_imageFilePaths = imageFilePaths;
}

int ImagePointDataSet::noImages() const
{
	return _imageFilePaths.size();
}

QMap<QString, QVariant> ImagePointDataSet::imageSizes() const
{
	auto imageSizes = QMap<QString, QVariant>();

	foreach(const QString& imageName, _imageSizes.keys()) {
		imageSizes[imageName] = QVariant(_imageSizes[imageName]);
	}

	return imageSizes;
}

QSize ImagePointDataSet::imageSize(const QString& imageName) const
{
	if (!_imageSizes.contains(imageName))
		return QSize();

	return _imageSizes[imageName];
}

void ImagePointDataSet::setImageSize(const QString& imageName, const QSize & imageSize)
{
	_imageSizes[imageName] = imageSize;
}

int ImagePointDataSet::noDimensions() const
{
	return _noDimensions;
}

void ImagePointDataSet::setNoDimensions(const int& noDimensions)
{
	_noDimensions = noDimensions;
}

QStringList ImagePointDataSet::dimensionNames() const
{
	return _dimensionNames;
}

void ImagePointDataSet::setDimensionNames(const QStringList & dimensionNames)
{
	_dimensionNames = dimensionNames;
}

FloatVector& ImagePointDataSet::pointsData()
{
	return _pointsData;
}

FloatVector& ImagePointDataSet::create()
{
	_pointsData.clear();
	_pointsData.resize(noPointsTotal());

	return _pointsData;
}

int ImagePointDataSet::noPointsPerDimension() const
{
	auto noPoints = 0;

	foreach(const QSize& imageSize, _imageSizes) {
		noPoints += imageSize.width() * imageSize.height();
	}

	return noPoints;
}

int ImagePointDataSet::noPointsTotal() const
{
	switch (_type)
	{
		case ImageCollectionType::Sequence:
			return noImages() * noPointsPerDimension();

		case ImageCollectionType::Stack:
		case ImageCollectionType::MultiPartSequence:
			return noDimensions() * noPointsPerDimension();

		default:
			return 0;
	}
}

QDebug operator<<(QDebug dbg, ImagePointDataSet & imageDataSet)
{
	const auto noImages				= QString::number(imageDataSet.noImages());
	const auto noDimensions			= QString::number(imageDataSet.noDimensions());
	const auto noPoints				= QString::number(imageDataSet.pointsData().size());
	const auto noPointsPerDimension	= QString::number(imageDataSet.noPointsPerDimension());

	dbg << QString("Image dataset '%1' of type %2, %3 images, %4 dims, %5 pts and %6 pts/dim").arg(imageDataSet.name(), imageCollectionTypeName(imageDataSet.type()), noImages, noDimensions, noPoints, noPointsPerDimension);

	return dbg;
}
