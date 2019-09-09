#pragma once

#include "ImageCollection.h"

#include <QObject>
#include <QString>

class ImageCollections : public QObject {
	Q_OBJECT

public:
	ImageCollections(const ImageCollectionType& type);

	void load();

protected:
	virtual void loadImage(const QString& imageFilePath, const int& imageIndex, FloatVector& pointsData) = 0;

signals:

private:
	ImageCollectionType		_type;
	ImageCollectionMap		_imageCollectionMap;
};