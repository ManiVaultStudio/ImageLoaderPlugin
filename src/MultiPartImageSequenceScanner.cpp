#include "MultiPartImageSequenceScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

MultiPartImageSequenceScanner::MultiPartImageSequenceScanner() :
	ImageScanner(ImageCollectionType::MultiPartSequence)
{
	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tif";

	setImageTypes(imageTypes);
}

MultiPartImageSequenceScanner::~MultiPartImageSequenceScanner()
{
}

void MultiPartImageSequenceScanner::scan()
{
	emit beginScan();

	//_imageStacks.clear();
	/*
	scanDir(_directory);

	const auto noStacks		= _imageStacks.size();
	const auto hasStacks	= noStacks > 0;

	if (noStacks == 0) {
		emit message("No image stacks were found, try changing the directory");
	}
	else {
		emit message(QString("Found %1 image stack(s)").arg(noStacks));
	}

	emit endScan(_imageStacks);*/
}

void MultiPartImageSequenceScanner::scanDir(const QString& directory)
{
	/*
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

	auto nameFilters = QStringList();

	foreach(QString imageType, imageTypes())
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		const auto size = imageReader.size();

		if (size.width() > 0 && size.height() > 0) {
			const auto sizeString = QString("%1x%2").arg(QString::number(size.width()), QString::number(imageReader.size().height()));

			if (!_imageStacks.contains(sizeString)) {
				_imageStacks.insert(sizeString, QStringList());
				_imageStackFiles.insert(size, QStringList());
			}

			_imageStacks[sizeString] << path;
		}

		scanDir(path);
	}
	*/
}