#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

class QSettings;

/*!
	\class ImageStackScanner
	\inherits ImageScanner
	\brief Scans for image stacks
	\ingroup ImageLoaderPlugin
*/
class ImageStackScanner : public ImageScanner {
	Q_OBJECT

public:
	ImageStackScanner();

	void scan() override;
	void run() override;

private:
	void scanDir(const QString& directory);
};