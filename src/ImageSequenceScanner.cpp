#include "ImageSequenceScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageSequenceScanner::ImageSequenceScanner() :
	ImageScanner(ImageCollectionType::Sequence)
{
	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tif";

	setImageTypes(imageTypes);
}

ImageSequenceScanner::~ImageSequenceScanner()
{
}

QString ImageSequenceScanner::imageType() const
{
	return _imageType;
}

void ImageSequenceScanner::setImageType(const QString & imageType)
{
	_imageType = imageType;

	emit imageTypeChanged(_imageType);

	emit becameDirty();
}

QSize ImageSequenceScanner::imageSize() const
{
	return _imageSize;
}

void ImageSequenceScanner::setImageSize(const QSize & imageSize)
{
	_imageSize = imageSize;

	emit imageSizeChanged(_imageSize);

	emit becameDirty();
}

void ImageSequenceScanner::scanDir(const QString& directory, ImageCollection& imageCollection)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		scanDir(path, imageCollection);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(QStringList() << "*." + _imageType);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto fileName			= fileList.at(i);
		const auto imageFilePath	= QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);

		QImageReader imageReader(imageFilePath);

		if (imageReader.size() == _imageSize) {
			imageCollection.add(imageFilePath);

			emit message(QString("Found %1").arg(fileName));
		}
	}
}

void ImageSequenceScanner::scan()
{
	emit beginScan();

	emit message("Scanning for image files...");

	auto imageCollections	= ImageCollections(ImageCollectionType::Sequence);
	auto imageCollection	= ImageCollection(_imageSize);
	
	scanDir(_directory, imageCollection);

	/*
	const auto noImages = imageFilePaths.size();

	if (noImages > 0) {
		emit message(QString("Found %1 images").arg(noImages));
	}
	else {
		emit message("No images were found...");
	}
	*/

	imageCollections.map()[QDir(_directory).dirName()] = imageCollection;

	emit endScan(imageCollections);
}

/*
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
QStringList ImageSequenceLoader::dimensionNames() const
{
	auto dimensionNames = QStringList();

	for (int i = 1; i <= noDimensions(); i++) {
		dimensionNames << QString("dim_%1").arg(i);
	}

	return dimensionNames;
}

*/