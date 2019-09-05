#pragma once

#include "ImageCollection.h"

#include <QObject>
#include <QString>

class QSettings;

class ImageScan : public QObject {
	Q_OBJECT

public:
	ImageScan(QSettings* settings);

	QString	directory() const;
	void setDirectory(const QString& directory);
	
	QStringList	imageTypes() const;
	void setImageTypes(const QStringList& imageTypes);

public:
	virtual void scan() = 0;

signals:
	void becameDirty();
	void beginScan();
	void endScan();
	void directoryChanged(const QString& directory);
	void imageTypesChanged(const QStringList& imageTypes);

private:
	QSettings*		_settings;
	QString			_directory;
	QStringList		_imageTypes;
};