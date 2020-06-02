#include "ImageCollection.h"

#include <QDebug>

ImageCollection::Image::Image() :
	_shouldLoad(true),
	_filePath(),
	_dimensionName(),
	_sourceSize(),
	_targetSize()
{
}

bool ImageCollection::Image::shouldLoad() const
{
	return _shouldLoad;
}

void ImageCollection::Image::setShouldLoad(const bool& shouldLoad)
{
	_shouldLoad = shouldLoad;
}

void ImageCollection::Image::setShouldLoad(const QString& filePath)
{
	_filePath = filePath;
}

QString ImageCollection::Image::dimensionName() const
{
	return _dimensionName;
}

void ImageCollection::Image::setDimensionName(const QString& dimensionName)
{
	_dimensionName = dimensionName;
}

QSize ImageCollection::Image::sourceSize() const
{
	return _sourceSize;
}

void ImageCollection::Image::setSourceSize(const QSize& sourceSize)
{
	_sourceSize = sourceSize;
}

QSize ImageCollection::Image::targetSize() const
{
	return _targetSize;
}

void ImageCollection::Image::setTargetSize(const QSize& targetSize)
{
	_targetSize = targetSize;
}

QString ImageCollection::Image::filePath() const
{
	return _filePath;
}

ImageCollection::ImageCollection() :
	_name(),
	_searchDir(),
	_filePath(),
	_sourceSize(),
	_targetSize(),
	_images()
{
}

QString ImageCollection::name() const
{
	return _name;
}

void ImageCollection::setName(const QString& name)
{
	_name = name;
}

QString ImageCollection::searchDir() const
{
	return _searchDir;
}

void ImageCollection::setSearchDir(const QString& searchDir)
{
	_searchDir = searchDir;
}

QString ImageCollection::filePath() const
{
	return _filePath;
}

void ImageCollection::setFilePath(const QString& filePath)
{
	_filePath = filePath;
}

QSize ImageCollection::sourceSize() const
{
	return _sourceSize;
}

void ImageCollection::setSourceSize(const QSize& sourceSize)
{
	_sourceSize = sourceSize;
}

QSize ImageCollection::targetSize() const
{
	return _targetSize;
}

void ImageCollection::setTargetSize(const QSize& targetSize)
{
	_targetSize = targetSize;
}

void ImageCollection::add(const Image& image)
{
	_images << image;
}