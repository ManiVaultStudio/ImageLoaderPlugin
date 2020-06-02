#pragma once

#include "ImageScanner.h"

#include <QString>
#include <QSize>

class QSettings;

/**
 * Image sequence scanner class
 * This class provides functionality for discovering image sequences
 * @author Thomas Kroes
 */
class ImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	/** Default constructor */
	ImageSequenceScanner();

	/** Load image scanner settings */
	void loadSettings() override;

public:
	/** Scan for image sequences */
	void scan() override;

	/** Start scan thread */
	void run() override;

private:
	/**
	 * Scan directory
	 * @param directory Search directory
	 * @param imageCollection Resulting image collection
	 */
	//void scanDir(const QString& directory, ImageCollection& imageCollection);

private:
	bool		_square;				/** Whether the image(s) are square */
};