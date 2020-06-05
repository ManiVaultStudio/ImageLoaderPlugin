#include "ImageScanner.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageScanner::ImageScanner(const ImageData::Type& type) :
	Settings("LKEB/CGV", "HDPS", QString("Plugins/ImageLoader/%1/Scanner").arg(ImageData::typeName(type))),
	_type(type),
	_directory(),
	_previousDirectories(),
	_supportedImageTypes(),
	_initialized(false)
{
	auto supportedImageTypes = QStringList();

	supportedImageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

	setSupportedImageTypes(supportedImageTypes);
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

void ImageScanner::scan()
{
	std::vector<ImageCollection> sequences;

	QStringList nameFilters;

	for (const auto& supportedImageType : supportedImageTypes())
		nameFilters << "*." + supportedImageType;

	qDebug() << nameFilters;

	scanDir(_directory, nameFilters, sequences);

	auto& imageCollectionsModel = _imageLoaderPlugin->imageCollectionsModel();

	imageCollectionsModel.clear();
	imageCollectionsModel.insert(0, sequences);
}

auto ImageScanner::findImageCollection(std::vector<ImageCollection>& imageCollections, const QString& imageType, const QSize& imageSize)
{
	return std::find_if(imageCollections.begin(), imageCollections.end(), [&imageType, &imageSize](const auto& sequence) {
		return sequence.imageType(Qt::EditRole).toString() == imageType && sequence.sourceSize(Qt::EditRole).toSize() == imageSize;
	});
}

void ImageScanner::scanDir(const QString& directory, QStringList nameFilters, std::vector<ImageCollection>& sequences)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		scanDir(path, nameFilters, sequences);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto fileName = fileList.at(i);
		const auto imageFilePath = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);
		const auto imageType = QFileInfo(fileName).suffix();

		QImageReader imageReader(imageFilePath);

		const auto imageSize = imageReader.size();

		auto it = findImageCollection(sequences, imageType, imageSize);

		if (it == sequences.end()) {
			auto imageCollection = ImageCollection(_directory, imageType, imageSize);

			imageCollection.addImage(imageFilePath);

			sequences.push_back(imageCollection);
		}
		else {

			(*it).addImage(imageFilePath);
		}
	}
}