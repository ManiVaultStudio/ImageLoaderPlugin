#include "ImageCollection.h"

#include <QDebug>

ImageCollection::ImageCollection() :
	_size(),
	_filePaths()
{
}

ImageCollection::ImageCollection(const QSize& size) :
	_size(size),
	_filePaths()
{
}

ImageCollection::ImageCollection(const ImageCollection& other) :
	ImageCollection(other._size)
{
}

ImageCollection::~ImageCollection()
{
}

QSize ImageCollection::size() const
{
	return _size;
}

QStringList ImageCollection::filePaths() const
{
	return _filePaths;
}

void ImageCollection::add(const QString& filePath)
{
	_filePaths << filePath;
}