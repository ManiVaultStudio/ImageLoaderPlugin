#include "ImageScan.h"

#include <QDebug>
#include <QDir>

ImageScan::ImageScan(QSettings* settings) :
	_settings(settings)
{
}

QString ImageScan::directory() const
{
	return _directory;
}

void ImageScan::setDirectory(const QString& directory)
{
	_directory = directory;

	emit directoryChanged(_directory);
	emit becameDirty();
}

QStringList ImageScan::imageTypes() const
{
	return _imageTypes;
}

void ImageScan::setImageTypes(const QStringList& imageTypes)
{
	_imageTypes = imageTypes;

	emit imageTypesChanged(_imageTypes);
	emit becameDirty();
}