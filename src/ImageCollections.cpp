#include "ImageCollections.h"

#include <QDebug>

ImageCollections::ImageCollections(const ImageCollectionType& type) :
	_type(type),
	_map()
{
}

ImageCollections::ImageCollections(const ImageCollections& other) :
	ImageCollections(other._type)
{
}

ImageCollections::~ImageCollections()
{
}
