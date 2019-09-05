#include "ImageStack.h"
#include "ImageUtilities.h"

#include <QDebug>
#include <QFileInfo>

ImageStack::ImageStack() :
	ImageCollection(ImageCollection::Type::Stack)
{
}

QString ImageStack::directory() const
{
	return _directory;
}

void ImageStack::setDirectory(const QString& directory)
{
	_directory = directory;

	emit directoryChanged(_directory);
	emit becameDirty();
}

QStringList ImageStack::imageTypes() const
{
	return _imageTypes;
}

void ImageStack::setImageTypes(const QStringList & imageTypes)
{
	_imageTypes = imageTypes;

	emit becameDirty();
}

QStringList ImageStack::dimensionNames() const
{
	auto dimensionNames = QStringList();
	/*
	foreach(const QString& imageFilePath, _imageFilePaths) {
		dimensionNames << QFileInfo(imageFilePath).fileName();
	}
	*/
	return dimensionNames;
}

int ImageStack::noDimensions() const
{
	return noImages();
}

void ImageStack::add(const QString & imageFilePath)
{
//	_imageFilePaths.append(imageFilePath);
}

void ImageStack::scan()
{
	emit beginScan();

	emit message("Scanning for images...");

	/*
	reset();

	scanDir(_directory);

	if (noImages() > 0) {
		emit message(QString("Found %1 images").arg(noImages()));
	}
	else {
		emit message("No images were found, try changing the directory, image type or dimensions");
	}
	*/

	emit endScan();
}

void ImageStack::scanDir(const QString &directory)
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

	foreach(QString imageType, _imageTypes)
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		// qDebug() << imageReader.size();

		const auto size = imageReader.size();

		if (size.width() > 0 && size.height() > 0) {
			const auto sizeString = QString("%1x%2").arg(QString::number(imageReader.size().width()), QString::number(imageReader.size().height()));

			if (!_stacks.contains(sizeString)) {
				_stacks.insert(sizeString, QSharedPointer<ImageStack>(new ImageStack(imageReader.size())));
			}

			_stacks[sizeString]->add(path);
		}

		scanDir(path);
	}
	*/
}

void ImageStack::load()
{
	/*
	emit beginLoad();

	std::vector<float> pointsData;

	// qDebug() << _imageFilePaths;

	pointsData.resize(noImages() * noPixels());

	qDebug() << pointsData.size();

	foreach(const QString &imageFilePath, _imageFilePaths) {
		const auto imageIndex	= _imageFilePaths.indexOf(imageFilePath);
		const auto done			= imageIndex + 1;
		const auto percentage	= 100.0f * (done / static_cast<float>(noImages()));

		loadImage(imageFilePath, imageIndex, pointsData);

		emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages()), QString::number(percentage, 'f', 1)));
	}

	// qDebug() << pointsData;

	emit message(QString("%1 image(s) loaded").arg(noImages()));

	emit endLoad(this, pointsData);
	*/
}

void ImageStack::loadImage(const QString & imageFilePath, const int& imageIndex, std::vector<float>& pointsData)
{
	/*
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* image = FreeImage_ConvertToGreyscale(freeImageLoad(imageFilePath));

	const auto image_type	= FreeImage_GetImageType(image);
	const auto imageWidth	= FreeImage_GetWidth(image);
	const auto imageHeight	= FreeImage_GetHeight(image);

	// qDebug() << image_type << pointsData.size();
	
	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (unsigned y = 0; y < imageHeight; y++) {
					const BYTE *const bits = FreeImage_GetScanLine(image, y);
					for (unsigned x = 0; x < imageWidth; x++) {
						const auto pixelId = y * imageWidth + x;
						const auto pointId = (pixelId * noDimensions()) + imageIndex;

						pointsData[pointId] = static_cast<float>(bits[x]);
					}
				}
			}

			break;
	}
	*/
}