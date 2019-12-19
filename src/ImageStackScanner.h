#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

class QSettings;

class ImageStackScanner : public ImageScanner {
	Q_OBJECT

public:
	ImageStackScanner();

	void scan();
	void run() override;

private:
	void scanDir(const QString& directory);
};