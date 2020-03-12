#include "Scanned.h"

#include <QDebug>

Scanned::Scanned(const ImageData::Type& type /*= ImageData::Type::Sequence*/) :
	_type(type),
	_map()
{
}

void Scanned::reset()
{
	_map.clear();
}

ImageData::Type Scanned::type() const
{
	return _type;
}

void Scanned::setType(const ImageData::Type& type)
{
	_type = type;
}

bool Scanned::loadable() const
{
	return _map.size() > 0;
}

QDebug operator<<(QDebug dbg, Scanned& scanned)
{
	dbg << ImageData::typeName(scanned.type());

	foreach(QString key, scanned.map().keys()) {
		dbg << key;
		dbg << scanned.map()[key];
	}

	return dbg;
}