#include "ImageStackScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageStackScanner::ImageStackScanner() :
	ImageScanner(ImageCollectionType::Stack)
{
	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tif";

	setImageTypes(imageTypes);
}

ImageStackScanner::~ImageStackScanner()
{
}

void ImageStackScanner::scan()
{
	emit beginScan();

	auto imageCollections = ImageCollections(ImageCollectionType::Stack);
	
	
	scanDir(_directory, imageCollections);

	const auto noStacks		= imageCollections.map().size();
	const auto hasStacks	= noStacks > 0;

	if (noStacks == 0) {
		emit message("No image stacks were found, try changing the directory");
	}
	else {
		emit message(QString("Found %1 image stack(s)").arg(noStacks));
	}

	emit endScan(imageCollections);
}

void ImageStackScanner::scanDir(const QString& directory, ImageCollections& imageCollections)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto imageFilePath = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		scanDir(imageFilePath, imageCollections);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);

	auto nameFilters = QStringList();

	foreach(QString imageType, imageTypes())
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto imageFilePath = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(imageFilePath);

		const auto size = imageReader.size();

		if (size.width() > 0 && size.height() > 0) {
			const auto sizeString = QString("%1x%2").arg(QString::number(size.width()), QString::number(imageReader.size().height()));

			
			if (!imageCollections.map().contains(sizeString)) {
				imageCollections.map()[sizeString] = ImageCollection(size);
			}
			
			imageCollections.map()[sizeString].add(imageFilePath);
		}

		scanDir(imageFilePath, imageCollections);
	}
}