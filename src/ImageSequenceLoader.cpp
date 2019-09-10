#include "ImageSequenceLoader.h"
#include "ImageUtilities.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImage.h>

ImageSequenceLoader::ImageSequenceLoader() :
	ImageCollectionLoader(ImageCollectionType::Sequence)
{
}


QDebug operator<<(QDebug dbg, const ImageSequenceLoader &sequence)
{
	//dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}