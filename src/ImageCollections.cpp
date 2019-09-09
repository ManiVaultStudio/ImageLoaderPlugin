#include "ImageCollections.h"

#include <QDebug>

ImageCollections::ImageCollections(const ImageCollectionType& type) :
	_type(type),
	_imageCollectionMap()
{
}