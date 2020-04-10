#include "ImageStackScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageStackScanner::ImageStackScanner() :
	ImageScanner(ImageData::Type::Stack)
{
	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tiff" << "tif";

	setSupportedImageTypes(imageTypes);

	for(const auto imageType : supportedImageTypes())
		_filter << "*." + imageType;
}

void ImageStackScanner::scan()
{
	emit beginScan();

	start();
}

void ImageStackScanner::run()
{
	if (!_initialized)
		return;

	_scanned->reset();

	scanDir(_directory);

	emit endScan(_scanned);

	const auto noStacks		= _scanned->map().size();
	const auto hasStacks	= noStacks > 0;

	if (noStacks == 0) {
		emit message("No image stacks were found, try changing the directory");
	}
	else {
		emit message(QString("Found %1 image stack(s)").arg(noStacks));
	}
}

void ImageStackScanner::scanDir(const QString& directory)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto imageFilePath = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		scanDir(imageFilePath);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);

	imageFiles.setNameFilters(_filter);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto imageFilePath = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));
		
		QImageReader imageReader(imageFilePath);

		const auto size = imageReader.size();

		if (size.width() > 0 && size.height() > 0) {
			const auto stackName = QString("%1:%2x%3").arg(QString(directory).replace(_directory, ""), QString::number(size.width()), QString::number(imageReader.size().height()));

			if (!_scanned->map().contains(stackName)) {
				_scanned->map()[stackName] = ImageCollection(size);
			}
			
			_scanned->map()[stackName].add(imageFilePath);
		}
		
		scanDir(imageFilePath);
	}
}