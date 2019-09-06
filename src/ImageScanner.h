#pragma once

#include "ImageCollectionLoader.h"

#include <QObject>
#include <QString>

class QSettings;

class ImageScanner : public QObject {
	Q_OBJECT

public:
	ImageScanner(const ImageCollectionType& type);
	~ImageScanner();

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
	void message(const QString& message);

protected:
	QSettings		_settings;
	QString			_directory;
	QStringList		_imageTypes;
};