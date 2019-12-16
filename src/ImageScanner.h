#pragma once

#include "ImageCollectionsLoader.h"

#include <QThread>
#include <QString>

class QSettings;

class ImageScanner : public QThread {
	Q_OBJECT

public:
	ImageScanner(const ImageCollectionType& type);

	void loadSettings();

	QString	directory() const;
	void setDirectory(const QString& directory);
	QStringList	previousDirectories() const;
	void setPreviousDirectories(const QStringList& previousDirectories);
	void addPreviousDirectory(const QString& previousDirectory);
	QStringList	imageTypes() const;
	void setImageTypes(const QStringList& imageTypes);

	ImageCollections& scanned();

public:
	virtual void scan() = 0;

signals:
	void settingsChanged();
	void beginScan();
	void endScan(const ImageCollections& scannedImageCollections);
	void directoryChanged(const QString& directory);
	void previousDirectoriesChanged(const QStringList& previousDirectories);
	void imageTypesChanged(const QStringList& imageTypes);
	void message(const QString& message);

protected:
	QSettings			_settings;
	QString				_directory;
	QStringList			_previousDirectories;
	QStringList			_imageTypes;
	ImageCollections	_scanned;
	bool				_initialized;
};