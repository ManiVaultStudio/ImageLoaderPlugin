#include "ImageCollection.h"

#include <QDebug>

ImageCollection::ImageCollection() :
	_imageFilePaths()
{
}

QStringList ImageCollection::imageFilePaths() const
{
	return _imageFilePaths;
}