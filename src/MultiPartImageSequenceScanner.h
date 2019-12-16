#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

class MultiPartImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	MultiPartImageSequenceScanner();

	void scan();
	void run() override;
	void scanDir(const QString& directory);
};