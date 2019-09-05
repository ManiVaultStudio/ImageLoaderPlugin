#include "ImageScanner.h"

#include <QDebug>
#include <QDir>
#include <QSettings>

ImageScanner::ImageScanner(const ImageCollection::Type& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(ImageCollection::typeName(type)))
{
	const auto directory = _settings.value("Scan/Directory", "").toString();

	if (QDir(directory).exists()) {
		_directory = _settings.value("Scan/Directory", "").toString();
	}
	else {
		_directory = "";
	}

	_directory	= _settings.value("Scan/Directory", "").toString();
	_imageTypes	= _settings.value("Scan/ImageTypes", "").toStringList();
}

ImageScanner::~ImageScanner()
{
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