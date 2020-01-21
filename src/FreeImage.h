#pragma once

#include <QString>
#include <QSize>

namespace fi {
#include <FreeImage.h>
}

/**
 * FreeImage class
 * Wrapper class for FreeImage functionality
 * @author Thomas Kroes
 */
class FreeImage
{
public:
	/**
	 * Load image from file path
	 * @param imageFilePath Image file path
	 * @return Handle to FreeImage bitmap
	 */
	static fi::FIBITMAP* freeImageLoad(const QString& imageFilePath);

	/**
	 * Open multi-bitmap (e.g. .tiff)
	 * @param imageFilePath Image file path
	 * @return Handle to FreeImage multi-bitmap
	 */
	static fi::FIMULTIBITMAP* freeImageOpenMultiBitmap(const QString& imageFilePath);

	/**
	 * Get number of pages in a FreeImage multi-bitmap
	 * @param imageFilePath Image file path
	 * @return Number of pages in FreeImage multi-bitmap
	 */
	static int freeImageGetPageCount(const QString& imageFilePath);

	/**
	 * Get size of a bitmap with FreeImage
	 * @param imageFilePath Image file path
	 * @return Size of the image
	 */
	static QSize freeImageGetSize(const QString& imageFilePath);
};