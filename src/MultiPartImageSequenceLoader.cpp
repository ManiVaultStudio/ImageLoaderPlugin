#include "MultiPartImageSequenceLoader.h"
#include "ImageUtilities.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImage.h>

MultiPartImageSequenceLoader::MultiPartImageSequenceLoader() :
	ImageCollectionLoader(ImageCollectionType::MultiPartSequence)
{
}

QStringList MultiPartImageSequenceLoader::dimensionNames() const
{
	auto dimensionNames = QStringList();

	for (int i = 1; i <= noDimensions(); i++) {
		dimensionNames << QString("dim_%1").arg(i);
	}

	return dimensionNames;
}

std::vector<float>& MultiPartImageSequenceLoader::pointsData()
{
	return _pointsData;
}

int MultiPartImageSequenceLoader::noDimensions() const
{
	return 10;
}

void MultiPartImageSequenceLoader::load()
{
}

void MultiPartImageSequenceLoader::loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);
	
	auto *image = FreeImage_ConvertToGreyscale(freeImageLoad(imageFilePath));
	
	const auto image_type = FreeImage_GetImageType(image);

	// qDebug() << image_type;

	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (unsigned y = 0; y <  FreeImage_GetHeight(image); y++) {
					const BYTE *const bits = FreeImage_GetScanLine(image, y);
					for (unsigned x = 0; x <  FreeImage_GetWidth(image); x++) {
						_pointsData.push_back(static_cast<float>(bits[x]));
					}
				}
			}
			break;
	}
}

QDebug operator<<(QDebug dbg, const MultiPartImageSequenceLoader& multipartImageSequence)
{
	//dbg << ;

	return dbg;
}