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

	ImageCollectionMap& map() {
		return _map;
	}

signals:

private:
	ImageCollectionType		_type;
	ImageCollectionMap		_map;
};