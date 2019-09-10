#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

class QSettings;

using ImageStackFiles = QPair<QSize, QStringList>;

class ImageStackScanner : public ImageScanner {
	Q_OBJECT

public:
	ImageStackScanner();
	~ImageStackScanner();

	void scan();
	void scanDir(const QString& directory);

private:
	QMap<QString, ImageStackFiles>	_imageStackFiles;
};