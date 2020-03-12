#pragma once

#include "ImageData/ImageData.h"
#include "Common.h"

/**
 * Payload class
 * The payload class contains loaded images for consumption by HDPS
 * @author Thomas Kroes
 */
class Payload
{
public:
	/**
	 * Constructor
	 * @param type Type of high-dimensional image collection
	 */
	Payload(const ImageData::Type& type = ImageData::Type::Undefined);

public:
	/** Returns the type of high-dimensional image collection */
	ImageData::Type type() const;

	/**
	 * Sets the type of high dimensional image collection
	 */
	void setType(const ImageData::Type& type);

	/** Returns the name of the payload */
	QString name() const;

	/** Sets the name of the payload
	 * @param name Name of the payload
	 */
	void setName(const QString& name);

	/** Returns the size of the images in the payload */
	QSize size() const;

	/** Sets the size of the images in the payload
	 * @param size Size of the images in the payload
	*/
	void setSize(const QSize& size);

	/** Returns the loaded images */
	std::vector<Image>& images();

	/** Returns the file paths of the loaded images */
	QStringList imageFilePaths() const;

	/**
	 * Sets the file paths of the loaded images
	 * @param imageFilePaths File paths of the loaded images
	 */
	void setImageFilePaths(const QStringList& imageFilePaths);

	/** Returns the number of images in the payload */
	std::uint32_t noImages() const;

	/**
	 * Add a loaded image
	 * @param noComponentsPerPixel Number of components per pixel
	 * @param imageFilePath File path of the image
	 * @return Image
	 */
	Image& add(const std::uint32_t& noComponentsPerPixel, const QString& imageFilePath);

protected:
	ImageData::Type			_type;				/** Type of payload e.g. sequence, stack */
	QString					_name;				/** Name of the payload */
	QSize					_size;				/** Size of the images */
	std::vector<Image>		_images;			/** Loaded images */
	QStringList				_imageFilePaths;	/** File paths of the loaded images */
};

/** Print the Payload class to the console */
QDebug operator<<(QDebug dbg, Payload& payload);