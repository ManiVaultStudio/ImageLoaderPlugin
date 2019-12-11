#include "ImageScanner.h"

#include <QDebug>
#include <QDir>
#include <QSettings>

ImageScanner::ImageScanner(const ImageCollectionType& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(imageCollectionTypeName(type))),
	_directory(),
	_previousDirectories(),
	_imageTypes(),
	_scanned(),
	_initialized(false)
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
	setPreviousDirectories(_settings.value("Scan/PreviousDirectories", QVariant::fromValue(QStringList())).toStringList());

	_initialized = true;
}

QString ImageScanner::directory() const
{
	return _directory;
}

void ImageScanner::setDirectory(const QString& directory)
{
	if (directory == _directory)
		return;

	_directory = directory;

	_settings.setValue("Scan/Directory", _directory);

	emit directoryChanged(_directory);
	
	addPreviousDirectory(_directory);

	emit becameDirty();
}

QStringList ImageScanner::previousDirectories() const
{
	return _previousDirectories;
}

void ImageScanner::setPreviousDirectories(const QStringList& previousDirectories)
{
	if (previousDirectories == _previousDirectories)
		return;

	_previousDirectories = previousDirectories;

	_settings.setValue("Scan/PreviousDirectories", _previousDirectories);

	emit previousDirectoriesChanged(_previousDirectories);

	emit becameDirty();
}

void ImageScanner::addPreviousDirectory(const QString& previousDirectory)
{
//	if (_previousDirectories.contains(previousDirectory))
//		return;

	auto previousDirectories = _previousDirectories << previousDirectory;

	setPreviousDirectories(_previousDirectories);
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
