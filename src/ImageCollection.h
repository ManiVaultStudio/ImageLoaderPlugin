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
			_size		= other._size;
			_filePaths	= other._filePaths;
		}

		return *this;
	}

	QSize size() const;
	QStringList	filePaths() const;

	void add(const QString& filePath);

protected:
	QSize			_size;
	QStringList		_filePaths;
};

using ImageCollectionMap = QMap<QString, ImageCollection>;