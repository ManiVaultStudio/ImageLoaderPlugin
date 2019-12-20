#pragma once

#include "ImageLoader.h"
#include "Settings.h"

#include <QThread>
#include <QString>

class ImageScanner : public QThread, public Settings {
	Q_OBJECT

public:
	ImageScanner(const ImageCollectionType& type);

	ImageCollectionType type() const;

	virtual void loadSettings();
	
	QString	directory() const;
	void setDirectory(const QString& directory, const bool& forceUpdate = false);
	QStringList	previousDirectories() const;
	void setPreviousDirectories(const QStringList& previousDirectories, const bool& forceUpdate = false);
	void addPreviousDirectory(const QString& previousDirectory);
	QStringList	imageTypes() const;
	void setImageTypes(const QStringList& imageTypes, const bool& forceUpdate = false);

	std::shared_ptr<ImageCollections> scanned();
	
public:
	virtual void scan() = 0;

signals:
	void settingsChanged();
	void beginScan();
	void endScan(std::shared_ptr<ImageCollections> scannedImageCollections);
	void directoryChanged(const QString& directory);
	void previousDirectoriesChanged(const QStringList& previousDirectories);
	void imageTypesChanged(const QStringList& imageTypes);
	void message(const QString& message);

protected:
	ImageCollectionType					_type;
	QString								_directory;
	QStringList							_previousDirectories;
	QStringList							_imageTypes;
	std::shared_ptr<ImageCollections>	_scanned;
	bool								_initialized;
};