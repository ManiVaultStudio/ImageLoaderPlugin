#pragma once

#include "ImageScanner.h"

#include <QString>
#include <QSize>

class QSettings;

/**
 * Image sequence scanner class
 *
 * This image scanner class is used to discovering image sequences
 *
 * @author Thomas Kroes
 */
class ImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public: // Construction

	/** Default constructor */
	ImageSequenceScanner();

public: // Miscellaneous

	/** Load image scanner settings */
	void loadSettings() override;

	/** Scan for images */
	virtual void scan() override;

	/**
	 * Finds a sequence in a collection of sequences based on image type and size
	 * @param sequences Collection of sequences
	 * @param imageType Type of image
	 * @param imageSize Image size
	 */
	static auto findImageCollection(std::vector<ImageCollection>& imageCollections, const QString& imageType, const QSize& imageSize);

private:

	/**
	 * Scan directory recursively
	 * @param directory Search directory
	 * @param nameFilters Image file types to filter
	 * @param sequences Loaded image sequences
	 */
	void scanDir(const QString& directory, QStringList nameFilters, std::vector<ImageCollection>& sequences);

private:
	bool		_square;				/** Whether the image(s) are square */
};