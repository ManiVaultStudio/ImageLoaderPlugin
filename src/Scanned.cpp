#include "Scanned.h"

#include <QDebug>

Scanned::Scanned(const ImageCollectionType& type /*= ImageCollectionType::Sequence*/) :
	_type(type),
	_map()
{
}

void Scanned::reset()
{
	_map.clear();
}

ImageCollectionType Scanned::type() const
{
	return _type;
}

void Scanned::setType(const ImageCollectionType& type)
{
	_type = type;
}

bool Scanned::loadable() const
{
	return _map.size() > 0;
}

QDebug operator<<(QDebug dbg, Scanned& scanned)
{
	dbg << imageCollectionTypeName(scanned.type());

	foreach(QString key, scanned.map().keys()) {
		dbg << key;
		dbg << scanned.map()[key];
	}

	return dbg;
}