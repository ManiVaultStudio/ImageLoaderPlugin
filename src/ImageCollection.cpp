#include "ImageCollection.h"

#include <QDebug>

ImageCollection::Image::Image() :
	_imageCollection(nullptr),
	_name(),
	_shouldLoad(true),
	_filePath(),
	_sourceSize(),
	_targetSize()
{
}

ImageCollection::Image::Image(const QString& name, const bool& shouldLoad, const QString& filePath, const QSize& sourceSize) :
	_imageCollection(nullptr),
	_name(name),
	_shouldLoad(shouldLoad),
	_filePath(filePath),
	_sourceSize(sourceSize),
	_targetSize(sourceSize)
{
}

ImageCollection* ImageCollection::Image::imageCollection()
{
	return _imageCollection;
}

void ImageCollection::Image::setImageCollection(ImageCollection* imageCollection)
{
	_imageCollection = imageCollection;
}

QString ImageCollection::Image::name() const
{
	return _name;
}

void ImageCollection::Image::setName(const QString& name)
{
	_name = name;
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

QString ImageCollection::Image::filePath() const
{
	return _filePath;
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

std::uint32_t ImageCollection::noImages() const
{
	return _images.count();
}

ImageCollection::Image* ImageCollection::image(const std::uint32_t& index)
{
	return &_images[index];
}

void ImageCollection::add(const Image& image)
{
	_images << image;

	_images.last().setImageCollection(this);
}