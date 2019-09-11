#pragma once

#include "SubsampleImageSettings.h"

#include <QObject>
#include <QString>
#include <QSize>

class ImageCollection {
public:
	ImageCollection();
	ImageCollection(const QSize& imageSize);
	ImageCollection(const ImageCollection& other);
	~ImageCollection();

	ImageCollection& operator=(const ImageCollection& other)
	{
		if (&other != this) {
			_imageSize		= other._imageSize;
			_noDimensions	= other._noDimensions;
			_imageFilePaths = other._imageFilePaths;
		}

		return *this;
	}

	int noImages() const;
	QSize imageSize() const;
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