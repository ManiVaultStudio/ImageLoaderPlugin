#pragma once

#include "ImageData/ImageData.h"
#include "ImageCollection.h"

/**
 * Scanned class
 * The image scanner (recursively) scans a search directory and groups references
 * to discovered images (e.g. sequence of 20x20 images and sequence of 128x128 images)
 * This class maps the collections of images that are eligible for loading
 * @author Thomas Kroes
 */
class Scanned {
public:
	/**
	 * Constructor
	 * @param type Type of image collection
	 */
	Scanned(const ImageCollectionType& type = ImageCollectionType::Sequence);

	/** Reset the contents */
	void reset();

	/** Returns the type of image collection */
	ImageCollectionType type() const;

	/**
	 * Sets the type of image collection
	 * @param type Type of image collection
	 */
	void setType(const ImageCollectionType& type);

	/**
	 * Map a collection name to an image collection
	 * @param name Name of the image image collection
	 * @param name imageCollection Image collection to map
	 */
	void set(const QString& name, const ImageCollection& imageCollection)
	{
		_map[name] = imageCollection;
	}

	/** Returns the image collection map */
	ImageCollectionMap& map() {
		return _map;
	}

	/** Returns whether there are any loadable image collections */
	bool loadable() const;

protected:
	ImageCollectionType		_type;		/** Type of image collection e.g. sequence, stack */
	ImageCollectionMap		_map;		/** Map of image collections */
};

/** Print the Scanned class to the console */
QDebug operator<<(QDebug dbg, Scanned& scanned);