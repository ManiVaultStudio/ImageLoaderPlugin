#include "ImageScanner.h"

#include <QDebug>
#include <QDir>

ImageScanner::ImageScanner(const ImageCollectionType& type) :
	QThread(),
	Settings("LKEB/CGV", "HDPS", QString("Plugins/ImageLoader/%1/Scanner").arg(imageCollectionTypeName(type))),
	_type(type),
	_directory(),
	_previousDirectories(),
	_imageTypes(),
	_scanned(std::make_shared<ImageCollections>(type)),
	_initialized(false)
{
}

ImageCollectionType ImageScanner::type() const
{
	return _type;
}

void ImageScanner::loadSettings()
{
	const auto directory = setting("Directory", "").toString();

	setDirectory(QDir(directory).exists() ? directory : "", true);
	setImageTypes(setting("ImageTypes", QStringList()).toStringList(), true);
	setPreviousDirectories(setting("PreviousDirectories", QVariant::fromValue(QStringList())).toStringList(), true);

	_initialized = true;
}

QString ImageScanner::directory() const
{
	return _directory;
}

void ImageScanner::setDirectory(const QString& directory, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && directory == _directory)
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

void ImageScanner::setPreviousDirectories(const QStringList& previousDirectories, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && previousDirectories == _previousDirectories)
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

QStringList ImageScanner::imageTypes() const
{
	return _imageTypes;
}

void ImageScanner::setImageTypes(const QStringList& imageTypes, const bool& forceUpdate /*= false*/)
{
	if (!forceUpdate && imageTypes == _imageTypes)
		return;

	_imageTypes = imageTypes;

	setSetting("ImageTypes", _imageTypes);

	qDebug() << "Set image types" << _imageTypes;

	emit imageTypesChanged(_imageTypes);

	emit settingsChanged();
}

std::shared_ptr<ImageCollections> ImageScanner::scanned()
{
	return _scanned;
}