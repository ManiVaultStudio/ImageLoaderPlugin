#include "ImageSequenceScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageSequenceScanner::ImageSequenceScanner() :
	ImageScanner(ImageCollectionType::Sequence),
	_imageType(),
	_imageSize(),
	_square(true)
{
	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

	setImageTypes(imageTypes);
}

void ImageSequenceScanner::loadSettings()
{
	setImageType(setting("ImageType", "jpg").toString(), true);
	setImageSize(setting("ImageSize", QSize(28, 28)).toSize(), true);
	setSquare(setting("Square", true).toBool(), true);

	ImageScanner::loadSettings();
}

QString ImageSequenceScanner::imageType() const
{
	return _imageType;
}

void ImageSequenceScanner::setImageType(const QString &imageType, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && imageType == _imageType)
		return;

	_imageType = imageType;

	setSetting("ImageType", _imageType);

	qDebug() << "Set image type" << _imageType;

	emit imageTypeChanged(_imageType);
	emit settingsChanged();
}

QSize ImageSequenceScanner::imageSize() const
{
	return _imageSize;
}

void ImageSequenceScanner::setImageWidth(const std::uint32_t& imageWidth, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && imageWidth == _imageSize.width())
		return;

	auto imageSize = _imageSize;

	imageSize.setWidth(imageWidth);

	if (_square) {
		imageSize.setHeight(imageWidth);
	}

	setImageSize(imageSize, forceUpdate);
}

void ImageSequenceScanner::setImageHeight(const std::uint32_t& imageHeight, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && imageHeight == _imageSize.height())
		return;

	if (_square)
		return;

	auto imageSize = _imageSize;

	imageSize.setHeight(imageHeight);

	setImageSize(imageSize, forceUpdate);
}

void ImageSequenceScanner::setImageSize(const QSize& imageSize, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && imageSize == _imageSize)
		return;

	_imageSize = imageSize;

	setSetting("ImageSize", _imageSize);

	qDebug() << "Set image size" << _imageSize;

	emit imageSizeChanged(_imageSize);
	emit settingsChanged();
}

bool ImageSequenceScanner::square() const
{
	return _square;
}

void ImageSequenceScanner::setSquare(const bool& square, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && square == _square)
		return;

	_square = square;

	setSetting("Square", _square);

	qDebug() << "Set square" << _square;

	emit squareChanged(_square);
	
	auto imageSize = _imageSize;

	imageSize.setHeight(imageSize.width());

	setImageSize(imageSize);

	emit settingsChanged();
}

void ImageSequenceScanner::scan()
{
	start();
}

void ImageSequenceScanner::run()
{
	if (!_initialized)
		return;

	emit beginScan();

	emit message("Scanning for image files...");

	_scanned->reset();

	auto imageCollection = ImageCollection(_imageSize);

	imageCollection.setNoDimensions(_imageSize.width() * _imageSize.height());

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
		const auto fileName = fileList.at(i);
		const auto imageFilePath = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);

		QImageReader imageReader(imageFilePath);

		if (imageReader.size() == _imageSize) {
			imageCollection.add(imageFilePath);

			emit message(QString("Found %1").arg(fileName));
		}
	}
}