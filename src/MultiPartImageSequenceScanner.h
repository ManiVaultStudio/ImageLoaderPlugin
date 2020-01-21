#pragma once

#include "ImageScanner.h"

/**
 * Multi-part Image stack scanner class
 * This class provides functionality for discovering multi-part image stacks
 */
class MultiPartImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	/** Default constructor */
	MultiPartImageSequenceScanner();

	/** Scan for multi-part image stacks */
	void scan() override;

	/** Start scan thread */
	void run() override;

private:
	/**
	 * Scan directory
	 * @param directory Search directory
	 */
	void scanDir(const QString& directory);
};