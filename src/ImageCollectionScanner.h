#pragma once

#include "ImageCollectionLoader.h"
#include "Settings.h"

/**
 * Image scanner class
 *
 * Provides functionality for (recursively) scanning a directory
 * for candidate image collections e.g. sequence and stack
 * @author Thomas Kroes
 */
class ImageCollectionScanner : public QObject, public Settings {
	Q_OBJECT

public:
	/**
	 * Default constructor */
	ImageCollectionScanner();

	/** Load image scanner settings */
	virtual void loadSettings();
	
	/**
	 * Sets the image loader plugin
	 * @param imageLoaderPlugin Pointer to image loader plugin (for interfacing with data models)
	 */
	void setImageLoaderPlugin(ImageLoaderPlugin* imageLoaderPlugin);

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

	/** Returns the support image types */
	QStringList	supportedImageTypes() const;

	/**
	 * Sets the supported image types
	 * @param imageTypes Types of images to search for
	 * @param notify Whether to notify others of this change
	 */
	void setSupportedImageTypes(const QStringList& supportedImageTypes, const bool& notify = false);

	/** Returns the result of the scanning process */
	//std::shared_ptr<Scanned> scanned();
	
	/**
	 * Finds an image collection based on image type and size
	 * @param imageCollections Image collections
	 * @param imageType Type of image
	 * @param imageSize Image size
	 */
	static auto findImageCollection(std::vector<ImageCollection>& imageCollections, const QString& imageType, const QSize& imageSize);

public:

	/** Scan for images */
	void scan();

private:

	/**
	 * Scan directory recursively
	 * @param directory Search directory
	 * @param nameFilters Image file types to filter
	 * @param sequences Loaded image sequences
	 */
	void scanDir(const QString& directory, QStringList nameFilters, std::vector<ImageCollection>& sequences);

signals:
	/** Signals that settings changed */
	void settingsChanged();

	/** Signals that scanning begun */
	void beginScan();

	/**
	 * Signals that scanning ended
	 * @param scanned Scanned images
	 */
	//void endScan(std::shared_ptr<Scanned> scanned);

	/**
	 * Signals that the search directory changed
	 * @param directory Search directory
	 */
	void directoryChanged(const QString& directory);

	/**
	 * Signals that the previous search directory changed
	 * @param previousDirectories Previous search directories
	 */
	void previousDirectoriesChanged(const QStringList& previousDirectories);

	/**
	 * Signals that the supported image types changed
	 * @param supportedImageTypes Supported image types
	 */
	void supportedImageTypesChanged(const QStringList& supportedImageTypes);

	/**
	 * Broadcasts a message
	 * @param message Message to broadcast
	 */
	void message(const QString& message);

protected:
	ImageLoaderPlugin*		_imageLoaderPlugin;			/** Pointer to image loader plugin (for interfacing with data models) */
	QString					_directory;					/** Top directory to search in (recursively) */
	QStringList				_previousDirectories;		/** List of previously visited directories */
	QStringList				_supportedImageTypes;		/** List of supported image types e.g. .tiff, .jpg */
	bool					_initialized;				/** Whether the scanner is initialized or not */
};