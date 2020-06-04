#include "ImageScanner.h"

#include <QDebug>
#include <QDir>

ImageScanner::ImageScanner(const ImageData::Type& type) :
	Settings("LKEB/CGV", "HDPS", QString("Plugins/ImageLoader/%1/Scanner").arg(ImageData::typeName(type))),
	_type(type),
	_directory(),
	_previousDirectories(),
	_supportedImageTypes(),
	_initialized(false)
{
}

ImageData::Type ImageScanner::type() const
{
	return _type;
}

void ImageScanner::loadSettings()
{
	const auto directory = setting("Directory", "").toString();

	setDirectory(QDir(directory).exists() ? directory : "", true);
	setSupportedImageTypes(setting("ImageTypes", QStringList()).toStringList(), true);
	setPreviousDirectories(setting("PreviousDirectories", QVariant::fromValue(QStringList())).toStringList(), true);

	_initialized = true;
}

void ImageScanner::setImageLoaderPlugin(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;
}

QString ImageScanner::directory() const
{
	return _directory;
}

void ImageScanner::setDirectory(const QString& directory, const bool& notify /*= false*/)
{
	if (!notify && directory == _directory)
		return;
	
	_directory = directory;

	setSetting("Directory", _directory);

	qDebug() << "Set directory" << _directory;

	emit directoryChanged(_directory);
	
	addPreviousDirectory(_directory);

	emit settingsChanged();
}

QStringList ImageScanner::previousDirectories() const
{
	return _previousDirectories;
}

void ImageScanner::setPreviousDirectories(const QStringList& previousDirectories, const bool& notify /*= false*/)
{
	if (!notify && previousDirectories == _previousDirectories)
		return;

	_previousDirectories = previousDirectories;

	setSetting("PreviousDirectories", _previousDirectories);

	qDebug() << "Set previous directories" << _previousDirectories;

	emit previousDirectoriesChanged(_previousDirectories);

	emit settingsChanged();
}

void ImageScanner::addPreviousDirectory(const QString& previousDirectory)
{
//	if (_previousDirectories.contains(previousDirectory))
//		return;

	auto previousDirectories = _previousDirectories << previousDirectory;

	setPreviousDirectories(_previousDirectories);
}

QStringList ImageScanner::supportedImageTypes() const
{
	return _supportedImageTypes;
}

void ImageScanner::setSupportedImageTypes(const QStringList& supportedImageTypes, const bool& notify /*= false*/)
{
	if (!notify && supportedImageTypes == _supportedImageTypes)
		return;

	_supportedImageTypes = supportedImageTypes;

	setSetting("ImageTypes", _supportedImageTypes);

	qDebug() << "Set image types" << _supportedImageTypes;

	emit supportedImageTypesChanged(_supportedImageTypes);

	emit settingsChanged();
}