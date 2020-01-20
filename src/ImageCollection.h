#pragma once

#include <QObject>
#include <QString>
#include <QSize>

/**
 * Image collection class
 * Represents a collection of images that have a common image size
 * This class does not contain the actual images, it merely contains a list of file paths
 */
class ImageCollection {
public:
	/** Default constructor */
	ImageCollection();

	/**
	 * Constructor
	 * @param imageSize Size of the image(s)
	 */
	ImageCollection(const QSize& imageSize);

	/** Returns the number of image this collection points to */
	int noImages() const;

	/* Returns the image size */
	QSize imageSize() const;

	/**
	 * Sets the image size
	 * @param imageSize Image size
	 */
	void setImageSize(const QSize& imageSize);
	
	/** Returns the image file paths this collection references */
	QStringList	imageFilePaths() const;

	/**
	 * Add an image file path
	 * @param imageFilePath Absolute image file path
	 */
	void add(const QString& imageFilePath);

	/** Returns the number of dimensions this collection represents in high-dimensional image space */
	int noDimensions() const;

	/**
	 * Sets the number of dimensions in high-dimensional image space
	 */
	void setNoDimensions(const int& noDimensions);
	
protected:
	QSize			_imageSize;			/** Size of the image(s) */
	QStringList		_imageFilePaths;	/** Absolute file path(s) of the image(s) */
	int				_noDimensions;		/** Number of dimensions */
};

/** Print the contents of the image collection */
QDebug operator<<(QDebug dbg, const class ImageCollection& imageCollection);

using ImageCollectionMap = QMap<QString, ImageCollection>;