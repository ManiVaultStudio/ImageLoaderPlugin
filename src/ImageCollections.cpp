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

ImageCollectionType ImageCollections::type() const
{
	return _type;
}

QString ImageCollections::name() const
{
	return _name;
}

void ImageCollections::setName(const QString& name)
{
	if (name == _name)
		return;

	_name = name;

	emit nameChanged(_name);
}

FloatVector ImageCollections::pointsData()
{
	return _pointsData;
}

QDebug operator<<(QDebug dbg, const ImageCollections& imageCollections)
{
	dbg << imageCollectionTypeName(imageCollections.type());

	foreach(QString key, imageCollections.map().keys()) {
		dbg << key;
		dbg << imageCollections.map()[key];
	}

	return dbg;
}