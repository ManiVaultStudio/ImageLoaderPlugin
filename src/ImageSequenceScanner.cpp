#include "ImageSequenceScanner.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageSequenceScanner::ImageSequenceScanner() :
	ImageScanner(ImageCollectionType::Sequence),
	_imageTypeFilter(),
	_imageSizeFilter(),
	_square(true)
{
	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

	setImageTypes(imageTypes);
}

void ImageSequenceScanner::loadSettings()
{
	setImageTypeFilter(setting("ImageType", "jpg").toString(), true);
	setImageSizeFilter(setting("ImageSize", QSize(28, 28)).toSize(), true);
	setSquare(setting("Square", true).toBool(), true);

	ImageScanner::loadSettings();
}

QString ImageSequenceScanner::imageTypeFilter() const
{
	return _imageTypeFilter;
}

void ImageSequenceScanner::setImageTypeFilter(const QString& imageTypeFilter, const bool& notify /*= false*/)
{
	if (!notify && imageTypeFilter == _imageTypeFilter)
		return;

	_imageTypeFilter = imageTypeFilter;

	setSetting("ImageType", _imageTypeFilter);

	qDebug() << "Set image type" << _imageTypeFilter;

	emit imageTypeFilterChanged(_imageTypeFilter);
	emit settingsChanged();
}

QSize ImageSequenceScanner::imageSizeFilter() const
{
	return _imageSizeFilter;
}

void ImageSequenceScanner::setImageWidthFilter(const std::uint32_t& imageWidthFilter, const bool& notify /*= false*/)
{
	if (!notify && imageWidthFilter == _imageSizeFilter.width())
		return;

	auto imageSize = _imageSizeFilter;

	imageSize.setWidth(imageWidthFilter);

	if (_square) {
		imageSize.setHeight(imageWidthFilter);
	}

	setImageSizeFilter(imageSize, notify);
}

void ImageSequenceScanner::setImageHeightFilter(const std::uint32_t& imageHeightFilter, const bool& notify /*= false*/)
{
	if (!notify && imageHeightFilter == _imageSizeFilter.height())
		return;

	if (_square)
		return;

	auto imageSize = _imageSizeFilter;

	imageSize.setHeight(imageHeightFilter);

	setImageSizeFilter(imageSize, notify);
}

void ImageSequenceScanner::setImageSizeFilter(const QSize &imageSizeFilter, const bool& notify /*= false*/)
{
	if (!notify && imageSizeFilter == _imageSizeFilter)
		return;

	_imageSizeFilter = imageSizeFilter;

	setSetting("ImageSize", _imageSizeFilter);

	qDebug() << "Set image size" << _imageSizeFilter;

	emit imageSizeFilterChanged(_imageSizeFilter);
	emit settingsChanged();
}

bool ImageSequenceScanner::square() const
{
	return _square;
}

void ImageSequenceScanner::setSquare(const bool& square, const bool& notify /*= false*/)
{
	if (!notify && square == _square)
		return;

	_square = square;

	setSetting("Square", _square);

	qDebug() << "Set square" << _square;

	emit squareChanged(_square);
	
	auto imageSize = _imageSizeFilter;

	imageSize.setHeight(imageSize.width());

	setImageSizeFilter(imageSize);

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