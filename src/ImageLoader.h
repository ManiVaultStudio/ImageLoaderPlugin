#pragma once

#include "Settings.h"
#include "Scanned.h"
#include "Payload.h"
#include "SubsampleSettings.h"
#include "ColorSettings.h"

#include <QThread>
#include <QString>

namespace fi {
	#include <FreeImage.h>
}

/**
 * Image loader class
 * Class for loading image collections and producing an image payload for loading into HDPS
 */
class ImageLoader : public QThread, public Settings {
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param type Type of image collection e.g. sequence, stack
	 */
	ImageLoader(const ImageCollectionType& type);

	/** Returns the type of image collection */
	ImageCollectionType type() const;

	/** Returns the subsample image settings */
	SubsampleSettings& subsampleImageSettings();

	/** Returns the color settings */
	ColorSettings& colorSettings();

	/**
	 * Load scanned images
	 * @param scanned Scanned images
	 */
	void load(std::shared_ptr<Scanned> scanned);

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

	/**  Returns the scanned images */
	std::shared_ptr<Scanned> scanned() const;

	/**
	 * Sets the scanned images
	 * @param scanned Scanned images
	 */
	void setScanned(std::shared_ptr<Scanned> scanned);

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
	ImageCollectionType			_type;					/** Type of image collection e.g. sequence, stack */
	QString						_datasetName;			/** Name of the data set */
	SubsampleSettings			_subsampleSettings;		/** Image subsample settings */
	ColorSettings				_colorSettings;			/** Image color settings */
	std::shared_ptr<Scanned>	_scanned;				/** Scanned image collections */
};