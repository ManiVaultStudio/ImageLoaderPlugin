#pragma once

#include "ImageScanner.h"

class QSettings;

/**
 * Image stack scanner class
 * This class provides functionality for discovering image stacks
 * @author Thomas Kroes
 */
class ImageStackScanner : public ImageScanner {
	Q_OBJECT

public:
	/** Default constructor */
	ImageStackScanner();

	/** Scan for images */
	virtual void scan() override;

private:
	/**
	 * Scan directory
	 * @param directory Search directory
	 */
	 //void scanDir(const QString& directory);

private:
	QStringList		_filter;
};