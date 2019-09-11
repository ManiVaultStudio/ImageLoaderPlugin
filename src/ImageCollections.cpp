#include "ImageCollections.h"

#include <QDebug>

ImageCollections::ImageCollections(const ImageCollectionType& type) :
	_type(type),
	_datasetName(),
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

QString ImageCollections::datasetName() const
{
	return _datasetName;
}

void ImageCollections::setDatasetName(const QString& datasetName)
{
	if (datasetName == _datasetName)
		return;

	_datasetName = datasetName;

	emit datasetNameChanged(_datasetName);
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

QDebug operator<<(QDebug dbg, ImageCollections& imageCollections)
{
	dbg << imageCollectionTypeName(imageCollections.type());

	foreach(QString key, imageCollections.map().keys()) {
		dbg << key;
		dbg << imageCollections.map()[key];
	}

	return dbg;
}