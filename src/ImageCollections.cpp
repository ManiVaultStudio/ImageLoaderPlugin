#include "ImageCollections.h"

#include <QDebug>

ImageCollections::ImageCollections(const ImageCollectionType& type) :
	_type(type),
	_name(),
	_map(),
	_pointsData()
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

FloatVector& ImageCollections::pointsData()
{
	return _pointsData;
}

int ImageCollections::noDimensions() const
{
	if (_map.isEmpty())
		return 0;

	return _map.first().noDimensions();
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