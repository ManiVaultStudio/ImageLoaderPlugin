#pragma once

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
	
	QStringList	imageFilePaths() const;

	void add(const QString& imageFilePath);

	int noDimensions() const;
	void setNoDimensions(const int& noDimensions);
	
protected:
	QSize			_imageSize;			/*!< Size of the image(s) */
	QStringList		_imageFilePaths;	/*!< Absolute file path(s) of the image(s) */
	int				_noDimensions;		/*!< Number of dimensions */
};

QDebug operator<<(QDebug dbg, const class ImageCollection& imageCollection);

using ImageCollectionMap = QMap<QString, ImageCollection>;