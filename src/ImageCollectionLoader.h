#pragma once

#include "Settings.h"
#include "Scanned.h"
#include "Payload.h"

#include <QThread>
#include <QString>

namespace fi {
	#include <FreeImage.h>
}

class ImageLoaderPlugin;

/**
 * Image loader class
 * Class for loading image collections and producing an 
 * image payload for loading into HDPS
 * @author Thomas Kroes
 */
class ImageCollectionLoader : public QThread, public Settings {
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param type Type of image collection e.g. sequence, stack
	 */
	ImageCollectionLoader(ImageLoaderPlugin* imageLoaderPlugin, const ImageData::Type& type);

	/** Returns the type of image collection */
	ImageData::Type type() const;

	/**
	 * Starts the loader thread
	 */
	void run() override;

public:
	/** Returns the dataset name */
	QString datasetName() const;

	/**
	 * Sets the dataset name
	 * @param datasetName Name of the dataset
	 * @param notify Whether to notify others of this change
	 */
	void setDatasetName(const QString& datasetName, const bool& notify = false);

private:
	/**
	 * Loads a FreeImage bitmap and adds it to the payload for HDPS
	 * @param bitmap FreeImage bitmap
	 * @param payload Payload which contains the loaded images (for consumption by HDPS)
	 * @param imageFilePath File path of the image
	 * @param dimensionName Name of the dimension
	 */
	void loadBitmap(fi::FIBITMAP* bitmap, Payload* payload, const QString& imageFilePath, const QString& dimensionName = "");

signals:
	/** Signals that loader settings changed */
	void settingsChanged();

	/**
	 * Signals that the dataset name changed
	 * @param datasetName Name of the dataset
	 */
	void datasetNameChanged(const QString& datasetName);

	/** Signals that the loading process begins  */
	void beginLoad();

	/**
	 * Signals that the loading process has ended
	 * @param payload Loaded images payload for consumption by Images data plugin
	 */
	void endLoad(std::shared_ptr<Payload> payload);

	/**
	 * Signals that an image is successfully loaded
	 * @param imageFilePath File path of the loaded image
	 * @param done Number of images that have been loaded thus far
	 * @param total Total number of images to load
	 */
	void imageLoaded(const QString &imageFilePath, const int& done, const int& total);

	/**
	 * Broadcasts a message
	 * @param message Message to broadcast
	 */
	void message(const QString& message);

private:
	ImageLoaderPlugin*			_imageLoaderPlugin;		/** Image loader plugin  */
	ImageData::Type				_type;					/** Type of image collection e.g. sequence, stack */
	QString						_datasetName;			/** Name of the data set */
};