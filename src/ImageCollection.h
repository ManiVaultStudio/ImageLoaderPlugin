#pragma once

#include "SubsampleImageSettings.h"

#include <QObject>
#include <QString>

class ImageCollection : public QObject {
	Q_OBJECT

public:
	ImageCollection();

	QStringList	imageFilePaths() const;

signals:

protected:
	QStringList				_imageFilePaths;
};

using ImageCollectionMap = QMap<QString, ImageCollection>;