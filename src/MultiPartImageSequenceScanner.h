#pragma once

#include "ImageScanner.h"

#include <QObject>
#include <QString>

class QSettings;

using ImageStackFiles = QPair<QSize, QStringList>;

class MultiPartImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	MultiPartImageSequenceScanner();
	~MultiPartImageSequenceScanner();

	void scan();
	void scanDir(const QString& directory);

signals:
	void endScan(QStringList& imageFilePaths);

private:
	QStringList	_scannedFiles;
};