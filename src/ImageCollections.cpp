#include "ImageCollections.h"

#include <QDebug>

ImageCollections::ImageCollections(const ImageCollectionType& type /*= ImageCollectionType::Sequence*/) :
	_type(type),
	_map()
{
	qRegisterMetaType<ImageCollections>("ImageCollections");
}

ImageCollections::~ImageCollections()
{
}

void ImageCollections::reset()
{
	_map.clear();
}

ImageCollectionType ImageCollections::type() const
{
	return _type;
}

bool ImageCollections::loadable() const
{
	return _map.size() > 0;
}

QDebug operator<<(QDebug dbg, ImageCollections& imageCollections)
{
	dbg << imageCollectionTypeName(imageCollections.type());

	foreach(QString key, imageCollections.map().keys()) {
		dbg << key;
		dbg << imageCollections.map()[key];
	}

	return dbg;
}