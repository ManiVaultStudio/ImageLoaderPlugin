#include "ImageSequenceLoader.h"
#include "ImageUtilities.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImage.h>

ImageSequenceLoader::ImageSequenceLoader() :
	ImageCollectionLoader(ImageCollectionType::Sequence),
	_directory(""),
	_imageType("jpg"),
	_imageSize(28, 28)
{
}

QString ImageSequenceLoader::directory() const
{
	return _directory;
}

QString ImageSequenceLoader::imageType() const
{
	return _imageType;

}
QSize ImageSequenceLoader::imageSize() const
{
	return _imageSize;
}

QStringList ImageSequenceLoader::dimensionNames() const
{
	auto dimensionNames = QStringList();

	for (int i = 1; i <= noDimensions(); i++) {
		dimensionNames << QString("dim_%1").arg(i);
	}

	return dimensionNames;
}

int ImageSequenceLoader::noDimensions() const
{
	return _imageSize.width() * _imageSize.height();
}

void ImageSequenceLoader::setDirectory(const QString & directory)
{
	_directory = directory;

	emit directoryChanged(_directory);
	emit becameDirty();
}

void ImageSequenceLoader::setImageType(const QString & imageType)
{
	_imageType = imageType;

	emit imageTypeChanged(_imageType);
	emit becameDirty();
}

void ImageSequenceLoader::setImageSize(const QSize & imageSize)
{
	_imageSize = imageSize;

	emit imageSizeChanged(_imageSize);
	emit becameDirty();
}

void ImageSequenceLoader::scan()
{
	emit beginScan();
	
	emit message("Scanning for image files...");

	reset();

	scanDir(_directory);

	if (noImages() > 0) {
		emit message(QString("Found %1 images").arg(noImages()));
	} else {
		emit message("No images were found, try changing the directory, image type or dimensions");
	}

	emit endScan();
}

void ImageSequenceLoader::load()
{
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
}

void ImageSequenceLoader::addFile(const QString &imageFilePath)
{
	_imageFilePaths.append(imageFilePath);

	emit message(QString("Found %1").arg(QFileInfo(imageFilePath).fileName()));
}

void ImageSequenceLoader::scanDir(const QString &directory)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		scanDir(path);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(QStringList() << "*." + _imageType);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		if (imageReader.size() == _imageSize) {
			addFile(path);
			scanDir(path);
		}
	}
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
	dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}