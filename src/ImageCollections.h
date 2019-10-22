#pragma once

#include "ImageData/ImageData.h"
#include "ImageCollection.h"

#include <QString>

class ImageCollections {
public:
	ImageCollections(const ImageCollectionType& type = ImageCollectionType::Sequence);
	~ImageCollections();

	void load();

	void reset();

	void set(const QString& name, const ImageCollection& imageCollection)
	{
		_map[name] = imageCollection;
	}

	ImageCollectionMap& map() {
		return _map;
	}

	ImageCollectionType type() const;

	bool loadable() const;

protected:
	ImageCollectionType		_type;
	ImageCollectionMap		_map;
};

QDebug operator<<(QDebug dbg, ImageCollections& imageCollections);