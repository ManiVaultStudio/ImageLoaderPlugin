#pragma once

#include "SubsampleImageSettings.h"

#include <QObject>
#include <QString>
#include <QSize>

class ImageCollection {
public:
	ImageCollection();
	ImageCollection(const QSize& imageSize);

	int noImages() const;
	QSize imageSize() const;
	void setImageSize(const QSize& imageSize);
	int noDimensions() const;
	void setNoDimensions(const int& noDimensions);
	QStringList	imageFilePaths() const;

	void add(const QString& imageFilePath);

protected:
	QSize			_imageSize;
	int				_noDimensions;
	QStringList		_imageFilePaths;
};

QDebug operator<<(QDebug dbg, const class ImageCollection& imageCollection);

using ImageCollectionMap = QMap<QString, ImageCollection>;