#include "ImageStack.h"

#include <QDebug>

#include <FreeImage.h>

ImageStack::ImageStack(const QSize& size /*= QSize()*/) :
	_size(size),
	_imageFilePaths()
{
}

ImageStack::ImageStack(const ImageStack &other)
{
}

ImageStack::~ImageStack()
{
}

QStringList ImageStack::imageFilePaths() const
{
	return _imageFilePaths;
}

int ImageStack::noDimensions() const
{
	return _size.width() * _size.height();
}

void ImageStack::add(const QString & imageFilePath)
{
	_imageFilePaths.append(imageFilePath);
}

void ImageStack::load()
{
	qDebug() << "Loading images ";

	emit beginLoad(this);

	std::vector<float> pointsData;

	/*
	pointsData.clear();

	const auto total = _imageFilePaths.size();

	foreach(const QString &imageFilePath, _imageFilePaths) {
		loadImage(imageFilePath);

		const auto done = _imageFilePaths.indexOf(imageFilePath) + 1;
		const auto percentage = 100.0f * (done / (float)total);

		emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
	}

	// qDebug() << pointsData;

	emit message(QString("%1 image(s) loaded").arg(total));
	*/

	emit endLoad(this, pointsData);
}

void ImageStack::loadImage(const QString & imageFilePath, std::vector<float>& pointsData)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto *image = FreeImage_ConvertToGreyscale(FreeImage_Load(format, imageFilePath.toUtf8()));

	unsigned x, y;

	const auto image_type = FreeImage_GetImageType(image);

	// qDebug() << image_type;

	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (y = 0; y < FreeImage_GetHeight(image); y++) {
					BYTE *bits = (BYTE *)FreeImage_GetScanLine(image, y);
					for (x = 0; x < FreeImage_GetWidth(image); x++) {
						pointsData.push_back((float)bits[x]);
					}
				}
			}
			break;

		case FIT_UINT16:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				unsigned short *bits = (unsigned short *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_INT16:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				short *bits = (short *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_UINT32:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				DWORD *bits = (DWORD *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_INT32:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				LONG *bits = (LONG *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_FLOAT:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				float *bits = (float *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_DOUBLE:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				double *bits = (double*)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_RGB16:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				FIRGB16 *bits = (FIRGB16 *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x].red);
				}
			}
			break;

		case FIT_RGBF:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				FIRGBF *bits = (FIRGBF *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x].red);
				}
			}
			break;

		case FIT_RGBA16:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				FIRGBA16 *bits = (FIRGBA16 *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x].red);
				}
			}
			break;

		case FIT_RGBAF:
			for (y = 0; y < FreeImage_GetHeight(image); y++) {
				FIRGBAF *bits = (FIRGBAF *)FreeImage_GetScanLine(image, y);
				for (x = 0; x < FreeImage_GetWidth(image); x++) {
					pointsData.push_back((float)bits[x].red);
				}
			}
			break;
	}
}