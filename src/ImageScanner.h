#pragma once

#include "ImageLoader.h"
#include "Settings.h"

#include <QThread>

/**
 * Image scanner class
 * Provides functionality for (recursively) scanning a directory for candidate images
 */
class ImageScanner : public QThread, public Settings {
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param type Type of image collection e.g. sequence, stack
	 */
	ImageScanner(const ImageCollectionType& type);

	/** Returns the type of image collection */
	ImageCollectionType type() const;

	/** Load image scanner settings */
	virtual void loadSettings();
	
	/** Returns the search directory */
	QString	directory() const;

	/**
	 * Sets the search directory
	 * @param directory Search directory
	 * @param notify Whether to notify others of this change
	 */
	void setDirectory(const QString& directory, const bool& notify = false);

	/** Returns previously searched directories */
	QStringList	previousDirectories() const;

	/**
	 * Sets previously searched directories
	 * @param previousDirectories Previously searched directories
	 * @param notify Whether to notify others of this change
	 */
	void setPreviousDirectories(const QStringList& previousDirectories, const bool& notify = false);

	/**
	 * Adds a previously searched directory
	 * @param previousDirectory Previously searched directory
	 */
	void addPreviousDirectory(const QString& previousDirectory);

	/** Returns the types of image this image scanner searches for */
	QStringList	imageTypes() const;

	/**
	 * Sets the types of image this image scanner searches for
	 * @param imageTypes Types of images to search for
	 * @param notify Whether to notify others of this change
	 */
	void setImageTypes(const QStringList& imageTypes, const bool& notify = false);

	/** Returns the result of the scanning process */
	std::shared_ptr<Scanned> scanned();
	
public:
	/** Scan for images */
	virtual void scan() = 0;

signals:
	/** Signals that settings changed */
	void settingsChanged();

	/** Signals that scanning has begun */
	void beginScan();

	/**
	 * Signals that scanning has ended
	 * @param scanned Scanned images
	 */
	void endScan(std::shared_ptr<Scanned> scanned);

	/**
	 * Signals that the search directory has changed
	 * @param directory Search directory
	 */
	void directoryChanged(const QString& directory);

	/**
	 * Signals that the previous search directory has changed
	 * @param previousDirectories Previous search directories
	 */
	void previousDirectoriesChanged(const QStringList& previousDirectories);

	/**
	 * Signals that the image types have changed
	 * @param imageTypes Image types
	 */
	void imageTypesChanged(const QStringList& imageTypes);

	/**
	 * Broadcasts a message
	 * @param message Message to broadcast
	 */
	void message(const QString& message);

protected:
	ImageCollectionType			_type;						/** Type of image collection e.g. sequence, stack */
	QString						_directory;					/** Top directory to search in (recursively) */
	QStringList					_previousDirectories;		/** List of previously visited directories */
	QStringList					_imageTypes;				/** Image types to search for e.g. .tiff, .jpg */
	std::shared_ptr<Scanned>	_scanned;					/** Result of scanning */
	bool						_initialized;				/** Whether the scanner is initialized or not */
};