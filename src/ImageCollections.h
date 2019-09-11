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
			_type			= other._type;
			_datasetName	= other._datasetName;
			_map			= other._map;
			_pointsData		= other._pointsData;
		}

		return *this;
	}

	void load();

	void set(const QString& name, const ImageCollection& imageCollection)
	{
		_map[name] = imageCollection;
	}

	ImageCollectionMap& map() {
		return _map;
	}

	ImageCollectionType type() const;

	QString datasetName() const;
	void setDatasetName(const QString& datasetName);

	FloatVector& pointsData();

	int noDimensions() const;

signals:
	void datasetNameChanged(const QString&);

protected:
	ImageCollectionType		_type;
	QString					_datasetName;
	ImageCollectionMap		_map;
	FloatVector				_pointsData;
};

QDebug operator<<(QDebug dbg, ImageCollections& imageCollections);