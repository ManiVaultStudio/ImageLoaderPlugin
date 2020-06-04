#include "ImageSequenceScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageSequenceScanner::ImageSequenceScanner() :
	ImageScanner(ImageData::Type::Sequence),
	_square(true)
{
	auto supportedImageTypes = QStringList();

	supportedImageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

	setSupportedImageTypes(supportedImageTypes);
}

void ImageSequenceScanner::loadSettings()
{
	ImageScanner::loadSettings();
}

void ImageSequenceScanner::scan()
{
//	start();
}

/*
void ImageSequenceScanner::run()
{
	
	if (!_initialized)
		return;

	emit beginScan();

	emit message("Scanning for image files...");

	_scanned->reset();

	auto imageCollection = ImageCollection(_imageSizeFilter);

	imageCollection.setNoDimensions(_imageSizeFilter.width() * _imageSizeFilter.height());

	scanDir(_directory, imageCollection);

	const auto noImages = imageCollection.noImages();

	if (noImages > 0) {
		const auto datasetName = QDir(_directory).dirName();

		_scanned->set(datasetName, imageCollection);

		emit message(QString("Found %1 images").arg(noImages));
	}
	else {
		emit message("No images were found...");
	}

	emit endScan(_scanned);
	
}
*/

/*
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
	imageFiles.setNameFilters(QStringList() << "*." + _imageTypeFilter);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto fileName = fileList.at(i);
		const auto imageFilePath = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);

		QImageReader imageReader(imageFilePath);

		if (imageReader.size() == _imageSizeFilter) {
			imageCollection.add(imageFilePath);

			emit message(QString("Found %1").arg(fileName));
		}
	}
	
}
*/