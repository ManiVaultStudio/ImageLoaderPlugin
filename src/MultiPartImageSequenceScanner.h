#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

/*!
	\class MultiPartImageSequenceScanner
	\inherits ImageScanner
	\brief Scans for multi-part image stacks
	\ingroup ImageLoaderPlugin
*/
class MultiPartImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	MultiPartImageSequenceScanner();

	void scan() override;
	void run() override;

private:
	void scanDir(const QString& directory);
};