#include "ImageCollectionScanner.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

namespace fi {
	#include <FreeImage.h>
}

ImageCollectionScanner::ImageCollectionScanner() :
	Settings("LKEB/CGV", "HDPS", "Plugins/ImageLoader/%1/Scanner"),
	_directory(),
	_separateByDirectory(false),
	_previousDirectories(),
	_supportedImageTypes(),
	_initialized(false)
{
	auto supportedImageTypes = QStringList();

	supportedImageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

	setSupportedImageTypes(supportedImageTypes);
}

void ImageCollectionScanner::loadSettings()
{
	const auto directory = setting("Directory", "").toString();

	setDirectory(QDir(directory).exists() ? directory : "", true);
	setSeparateByDirectory(setting("SeparateByDirectory", true).toBool());
	setSupportedImageTypes(setting("ImageTypes", QStringList()).toStringList(), true);

	_initialized = true;
}

void ImageCollectionScanner::setImageLoaderPlugin(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;
}

QString ImageCollectionScanner::directory() const
{
	return _directory;
}

void ImageCollectionScanner::setDirectory(const QString& directory, const bool& notify /*= false*/)
{
	if (!notify && directory == _directory)
		return;
	
	_directory = directory;

	setSetting("Directory", _directory);

	qDebug() << "Set directory" << _directory;

	emit directoryChanged(_directory);
	
	emit settingsChanged();

	if (_initialized)
		scan();
}

bool ImageCollectionScanner::separateByDirectory() const
{
	return _separateByDirectory;
}

void ImageCollectionScanner::setSeparateByDirectory(const bool& separateByDirectory, const bool& notify /*= false*/)
{
	if (separateByDirectory == _separateByDirectory)
		return;

	_separateByDirectory = separateByDirectory;

	setSetting("SeparateByDirectory", _separateByDirectory);

	emit separateByDirectoryChanged(_separateByDirectory);
	emit settingsChanged();

	if (_initialized)
		scan();
}

QStringList ImageCollectionScanner::supportedImageTypes() const
{
	return _supportedImageTypes;
}

void ImageCollectionScanner::setSupportedImageTypes(const QStringList& supportedImageTypes, const bool& notify /*= false*/)
{
	if (!notify && supportedImageTypes == _supportedImageTypes)
		return;

	_supportedImageTypes = supportedImageTypes;

	setSetting("ImageTypes", _supportedImageTypes);

	qDebug() << "Set image types" << _supportedImageTypes;

	emit supportedImageTypesChanged(_supportedImageTypes);

	emit settingsChanged();

	if (_initialized)
		scan();
}

void ImageCollectionScanner::scan()
{
	std::vector<ImageCollection> imageCollections;

	QStringList nameFilters;

	for (const auto& supportedImageType : supportedImageTypes())
		nameFilters << "*." + supportedImageType;

	scanDir(_directory, nameFilters, imageCollections);

	for (auto& imageCollection : imageCollections)
		imageCollection.computeDatasetName();

	auto& imageCollectionsModel = _imageLoaderPlugin->imageCollectionsModel();

	imageCollectionsModel.clear();
	imageCollectionsModel.insert(0, imageCollections);
}

auto ImageCollectionScanner::findImageCollection(std::vector<ImageCollection>& imageCollections, const QString& directory, const QString& imageType, const QSize& imageSize)
{
	return std::find_if(imageCollections.begin(), imageCollections.end(), [this, &directory, &imageType, &imageSize](const auto& imageCollection) {
		if (_separateByDirectory && imageCollection.directory(Qt::EditRole).toString() != directory)
			return false;

		if (imageCollection.imageType(Qt::EditRole).toString() != imageType)
			return false;

		if (imageCollection.sourceSize(Qt::EditRole).toSize() != imageSize)
			return false;

		return true;
	});
}

void ImageCollectionScanner::scanDir(const QString& directory, QStringList nameFilters, std::vector<ImageCollection>& imageCollections)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		scanDir(path, nameFilters, imageCollections);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto fileName			= fileList.at(i);
		const auto imageFilePath	= QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);
		const auto imageType		= QFileInfo(fileName).suffix();
		const auto rootDir			= QFileInfo(imageFilePath).absoluteDir().path();

		QImageReader imageReader(imageFilePath);

		const auto imageSize = imageReader.size();

		auto it = findImageCollection(imageCollections, rootDir, imageType, imageSize);

		if (it == imageCollections.end()) {
			auto imageCollection = ImageCollection(rootDir, imageType, imageSize);

			auto loadOne = true;

			if (imageType == "tiff") {
				imageReader.jumpToNextImage();

				const auto pageCount = imageReader.imageCount();

				if (pageCount > 1) {
					loadOne = false;

					for (int pageIndex = 0; pageIndex < pageCount; pageIndex++) {
						imageCollection.addImage(imageFilePath, pageIndex);
						//imageCollection.images().back().setDimensionName(QString("Dim").arg(pageIndex));
					}
				}
			}
			
			if (loadOne) {
				imageCollection.addImage(imageFilePath);
			}

			imageCollections.push_back(imageCollection);
		}
		else {

			(*it).addImage(imageFilePath);
		}
	}
}