#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

class MultiPartImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	MultiPartImageSequenceScanner();
	~MultiPartImageSequenceScanner();

	void scan();
	void scanDir(const QString& directory);
};