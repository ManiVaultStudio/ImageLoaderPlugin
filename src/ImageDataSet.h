#pragma once

#include "Common.h"

#include <QString>
#include <QSize>

class ImageDataSet {
public:
	ImageDataSet(const ImageCollectionType& type);

public:
	QString name() const;
	void setName(const QString& name);
	ImageCollectionType type() const;
	void setType(const ImageCollectionType& type);
	int noImages() const;
	void setNoImages(const int& noImages);
	QSize imageSize() const;
	void setImageSize(const QSize& imageSize);
	int noDimensions() const;
	void setNoDimensions(const int& noDimensions);
	QStringList dimensionNames() const;
	void setDimensionNames(const QStringList& dimensionNames);

	FloatVector& pointsData();

protected:
	ImageCollectionType		_type;
	QString					_name;
	int						_noImages;
	QSize					_imageSize;
	int						_noDimensions;
	QStringList				_dimensionNames;
	FloatVector				_pointsData;
};

QDebug operator<<(QDebug dbg, ImageDataSet& imageDataSet);