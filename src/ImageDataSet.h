#pragma once

#include "Common.h"

#include <QString>
#include <QSize>
#include <QMap>
#include <QVariant>

using ImageSizes = QMap<QString, QSize>;

class ImageDataSet {
public:
	ImageDataSet(const ImageCollectionType& type);

public:
	QString name() const;
	void setName(const QString& name);
	ImageCollectionType type() const;
	void setType(const ImageCollectionType& type);
	QStringList imageFilePaths() const;
	void setImageFilePaths(const QStringList& imageFilePaths);
	int noImages() const;
	QMap<QString, QVariant> imageSizes() const;
	QSize imageSize(const QString& imageName) const;
	void setImageSize(const QString& imageName, const QSize& imageSize);
	int noDimensions() const;
	void setNoDimensions(const int& noDimensions);
	QStringList dimensionNames() const;
	void setDimensionNames(const QStringList& dimensionNames);

	FloatVector& pointsData();

protected:
	ImageCollectionType		_type;
	QString					_name;
	QStringList				_imageFilePaths;
	ImageSizes				_imageSizes;
	int						_noDimensions;
	QStringList				_dimensionNames;
	FloatVector				_pointsData;
};

QDebug operator<<(QDebug dbg, ImageDataSet& imageDataSet);