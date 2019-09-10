#pragma once

#include "ImageCollection.h"

#include <QObject>
#include <QString>

class ImageCollections : public QObject {
	Q_OBJECT

public:
	ImageCollections(const ImageCollectionType& type);
	ImageCollections(const ImageCollections& other);
	~ImageCollections();

	ImageCollections& operator=(const ImageCollections& other)
	{
		if (&other != this) {
			_type	= other._type;
			_map	= other._map;
		}

		return *this;
	}

	void load();

	void set(const QString& name, const ImageCollection& imageCollection)
	{
		_map[name] = imageCollection;
	}

	const ImageCollectionMap& map() const {
		return _map;
	}

	ImageCollectionType type() const;

	FloatVector	pointsData();

signals:

private:
	ImageCollectionType		_type;
	ImageCollectionMap		_map;
	FloatVector				_pointsData;
};

QDebug operator<<(QDebug dbg, const class ImageCollections& imageCollections);