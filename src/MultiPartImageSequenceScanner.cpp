#include "MultiPartImageSequenceScanner.h"
#include "FreeImage.h"
#include "ImageData.h"

#include <QDebug>
#include <QDir>

MultiPartImageSequenceScanner::MultiPartImageSequenceScanner() :
	ImageScanner(ImageCollectionType::MultiPartSequence)
{
	auto imageTypes = QStringList();

	imageTypes << "tiff";

	setImageTypes(imageTypes);
}

void MultiPartImageSequenceScanner::scan()
{
	emit beginScan();

	_scanned.reset();

	auto imageFiles = QDir(_directory);

	imageFiles.setFilter(QDir::Files);

	auto nameFilters = QStringList();

	foreach(QString imageType, imageTypes())
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileNames = imageFiles.entryList();

	auto multiPartTiffs = QMap<int, QStringList>();

	for (int i = 0; i < fileNames.size(); ++i)
	{
		const auto filePath		= QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileNames.at(i));
		const auto pageCount	= FreeImage::freeImageGetPageCount(filePath);

		if (!multiPartTiffs.contains(pageCount)) {
			multiPartTiffs[pageCount] = QStringList();
		}

		multiPartTiffs[pageCount] << filePath;
	}

	multiPartTiffs.remove(1);

	if (multiPartTiffs.size() >= 1) {
		const auto imageFilePaths = multiPartTiffs.last();

		const auto noDimensions = multiPartTiffs.keys().last();

		foreach(const QString& imageFilePath, imageFilePaths) {
			auto imageCollection = ImageCollection(FreeImage::freeImageGetSize(imageFilePath));

			imageCollection.add(imageFilePath);
			imageCollection.setNoDimensions(noDimensions);

			_scanned.map()[imageFilePath] = imageCollection;
		}

		emit message(QString("Found %1 multipart image(s)").arg(imageFilePaths.size()));
	}
	else {
		emit message("No multipart images were found, try changing the directory");
	}

	emit endScan(_scanned);
}