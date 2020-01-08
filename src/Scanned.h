#pragma once

#include "ImageData/ImageData.h"
#include "ImageCollection.h"

#include <QString>

/*!
	\class Scanned
	\brief Contains the image collections that are eligible for loading
	\ingroup ImageLoaderPlugin
*/
class Scanned {
public:
	Scanned(const ImageCollectionType& type = ImageCollectionType::Sequence);

	void reset();

	ImageCollectionType type() const;
	void setType(const ImageCollectionType& type);

	void set(const QString& name, const ImageCollection& imageCollection)
	{
		_map[name] = imageCollection;
	}

	ImageCollectionMap& map() {
		return _map;
	}

	bool loadable() const;

protected:
	ImageCollectionType		_type;		/*!< Type of image collection e.g. sequence, stack */
	ImageCollectionMap		_map;		/*!< Map of image collections */
};

QDebug operator<<(QDebug dbg, Scanned& scanned);