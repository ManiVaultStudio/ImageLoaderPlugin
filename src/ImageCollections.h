#pragma once

#include "ImageData/ImageData.h"
#include "ImageCollection.h"

#include <QString>

class ImageCollections {
public:
	ImageCollections(const ImageCollectionType& type = ImageCollectionType::Sequence);

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
	ImageCollectionType		_type;
	ImageCollectionMap		_map;
};

QDebug operator<<(QDebug dbg, ImageCollections& imageCollections);