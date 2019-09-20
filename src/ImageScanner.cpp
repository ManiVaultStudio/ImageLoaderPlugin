#include "ImageScanner.h"

#include <QDebug>
#include <QDir>
#include <QSettings>

ImageScanner::ImageScanner(const ImageCollectionType& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(imageCollectionTypeName(type))),
	_directory(),
	_imageTypes(),
	_scanned()
{
}

ImageScanner::~ImageScanner()
{
}

void ImageScanner::loadSettings()
{
	const auto directory = _settings.value("Scan/Directory", "").toString();

	if (QDir(directory).exists()) {
		setDirectory(_settings.value("Scan/Directory", "").toString());
	}
	else {
		setDirectory("");
	}

	setImageTypes(_settings.value("Scan/ImageTypes", "").toStringList());
}

QString ImageScanner::directory() const
{
	return _directory;
}

void ImageScanner::setDirectory(const QString& directory)
{
	_directory = directory;

	_settings.setValue("Scan/Directory", directory);

	emit directoryChanged(_directory);
	
	emit becameDirty();
}

QStringList ImageScanner::imageTypes() const
{
	return _imageTypes;
}

void ImageScanner::setImageTypes(const QStringList& imageTypes)
{
	_imageTypes = imageTypes;

	_settings.setValue("Scan/ImageTypes", _imageTypes);

	emit imageTypesChanged(_imageTypes);

	emit becameDirty();
}

ImageCollections & ImageScanner::scanned()
{
	return _scanned;
}
