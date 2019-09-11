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
			_type		= other._type;
			_name		= other._name;
			_map		= other._map;
			_pointsData	= other._pointsData;
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

	QString name() const;
	void setName(const QString& name);

	FloatVector& pointsData();

	int noDimensions() const;

signals:
	void nameChanged(const QString&);

protected:
	ImageCollectionType		_type;
	QString					_name;
	ImageCollectionMap		_map;
	FloatVector				_pointsData;
};

QDebug operator<<(QDebug dbg, const class ImageCollections& imageCollections);