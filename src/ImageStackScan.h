#pragma once

#include "ImageScan.h"

#include <QObject>
#include <QString>

class QSettings;

using ImageStacks = QMap<QString, QStringList>;

class ImageStackScan : public ImageScan {
	Q_OBJECT

public:
	ImageStackScan(QSettings* settings);

	void scan();

signals:
	void becameDirty();
	void beginScan();
	void endScan(ImageStacks& imageStacks);
};