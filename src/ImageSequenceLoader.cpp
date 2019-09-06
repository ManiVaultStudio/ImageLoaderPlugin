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

void ImageSequenceLoader::load()
{
	/*
	emit beginLoad();

	emit message(QString("Loading %1 images").arg(noImages()));

	FloatVector pointsData;

	pointsData.clear();

	pointsData.reserve(noImages() * noDimensions());

	const auto total = noImages();

	foreach(const QString &imageFilePath, _imageFilePaths) {
		loadImage(imageFilePath, _imageFilePaths.indexOf(imageFilePath), pointsData);

		const auto done = _imageFilePaths.indexOf(imageFilePath) + 1;
		const auto percentage = 100.0f * (done / static_cast<float>(total));

		emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
	}

	emit message(QString("%1 image(s) loaded").arg(total));

	emit endLoad(pointsData);
	*/
}

void ImageSequenceLoader::loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData)
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
						pointsData.push_back(static_cast<float>(bits[x]));
					}
				}
			}
			break;
	}
}

QDebug operator<<(QDebug dbg, const ImageSequenceLoader &sequence)
{
	//dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}