#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

class QSettings;

using ImageStacks = QMap<QString, QStringList>;

class ImageStackScanner : public ImageScanner {
	Q_OBJECT

public:
	ImageStackScanner(QSettings* settings);

	void scan();
	void scanDir(const QString& directory);

signals:
	void becameDirty();
	void beginScan();
	void endScan(ImageStacks& imageStacks);
};