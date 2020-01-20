#pragma once

#include "ImageScanner.h"

#include <QString>
#include <QSize>

class QSettings;

/**
 * Image sequence scanner class
 * Scans for image sequences
 */
class ImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	/** Default constructor */
	ImageSequenceScanner();

	/** Load image scanner settings */
	void loadSettings() override;

public:
	/** Returns the image type filter */
	QString	imageTypeFilter() const;

	/**
	 * Sets the image type filter
	 * @param imageTypeFilter Image types to filter
	 * @param notify Whether to notify others of this change
	 */
	void setImageTypeFilter(const QString& imageTypeFilter, const bool& notify = false);

	/** Returns the image size filter */
	QSize imageSizeFilter() const;

	/**
	 * Sets the image width filter
	 * @param imageWidthFilter Image width filter
	 * @param notify Whether to notify others of this change
	 */
	void setImageWidthFilter(const std::uint32_t& imageWidthFilter, const bool& notify = false);

	/**
	 * Sets the image height filter
	 * @param imageHeightFilter Image height filter
	 * @param notify Whether to notify others of this change
	 */
	void setImageHeightFilter(const std::uint32_t& imageHeightFilter, const bool& notify = false);

	/**
	 * Sets the image size filter
	 * @param imageSizeFilter Image size filter
	 * @param notify Whether to notify others of this change
	 */
	void setImageSizeFilter(const QSize &imageSizeFilter, const bool& notify = false);

	/** Returns whether search width and height are equal */
	bool square() const;

	/**
	 * Sets whether width and height should be equal
	 * @param square Square images
	 */
	void setSquare(const bool& square, const bool& notify = false);

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
	void scanDir(const QString& directory, ImageCollection& imageCollection);

signals:
	/**
	 * Signals that the image type filter changed
	 * @param imageTypeFilter Image type filter
	 */
	void imageTypeFilterChanged(const QString& imageTypeFilter);

	/**
	 * Signals that the image size filter changed
	 * @param imageSizeFilter Image size filter
	 */
	void imageSizeFilterChanged(const QSize& imageSizeFilter);

	/**
	 * Signals that the square property changed
	 * @param square Whether width and height should be equal
	 */
	void squareChanged(const bool& square);

private:
	QString		_imageTypeFilter;		/** Type of image e.g. .tiff, .jpg */
	QSize		_imageSizeFilter;		/** Two-dimensional size of the image */
	bool		_square;				/** Whether the image(s) are square */
};