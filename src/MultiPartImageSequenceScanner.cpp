#include "MultiPartImageSequenceScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

MultiPartImageSequenceScanner::MultiPartImageSequenceScanner() :
	ImageScanner(ImageCollectionType::MultiPartSequence)
{
	auto imageTypes = QStringList();

	imageTypes << "tiff";

	setImageTypes(imageTypes);
}

MultiPartImageSequenceScanner::~MultiPartImageSequenceScanner()
{
}

void MultiPartImageSequenceScanner::scan()
{
	emit beginScan();

	_scannedFiles.clear();

	auto imageFiles = QDir(_directory);

	imageFiles.setFilter(QDir::Files);

	auto nameFilters = QStringList();

	foreach(QString imageType, imageTypes())
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileNames = imageFiles.entryList();

	auto imageFilePaths = QStringList();

	for (int i = 0; i < fileNames.size(); ++i)
	{
		imageFilePaths << QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileNames.at(i));
	}

	const auto noStacks		= imageFilePaths.size();
	const auto hasStacks	= noStacks > 0;

	if (noStacks == 0) {
		emit message("No multipart images were found, try changing the directory");
	}
	else {
		emit message(QString("Found %1 multipart image(s)").arg(noStacks));
	}

	emit endScan(imageFilePaths);
}