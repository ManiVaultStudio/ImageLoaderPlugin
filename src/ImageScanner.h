#pragma once

#include "ImageLoader.h"
#include "Settings.h"

#include <QThread>
#include <QString>

/*!
	\class ImageScanner
	\inherits QThread
	\inherits Settings
	\brief Abstract base scanner class for scanning images
	\ingroup ImageLoaderPlugin
*/
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

	std::shared_ptr<Scanned> scanned();
	
public:
	virtual void scan() = 0;

signals:
	void settingsChanged();
	void beginScan();
	void endScan(std::shared_ptr<Scanned> scanned);
	void directoryChanged(const QString& directory);
	void previousDirectoriesChanged(const QStringList& previousDirectories);
	void imageTypesChanged(const QStringList& imageTypes);
	void message(const QString& message);

protected:
	ImageCollectionType			_type;						/*!< Type of image collection e.g. sequence, stack */
	QString						_directory;					/*!< Top directory to search in (recursively) */
	QStringList					_previousDirectories;		/*!< List of previously visited directories */
	QStringList					_imageTypes;				/*!< Image types to search for e.g. tiff, jpg */
	std::shared_ptr<Scanned>	_scanned;					/*!< Result of scanning */
	bool						_initialized;				/*!< Whether the scanner is initialized or not */
};