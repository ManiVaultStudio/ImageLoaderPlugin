#include "ImageCollectionScanner.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>
#include <QMessageBox>
#include <QProgressDialog>

#include <stdexcept> // For runtime_error.

namespace fi {
	#include <FreeImage.h>
}

ImageCollectionScanner::ImageCollectionScanner() :
	Settings("LKEB/CGV", "HDPS", "Plugins/ImageLoader/%1/Scanner"),
	_directory(),
	_separateByDirectory(false),
	_previousDirectories(),
	_supportedImageTypes(),
	_filenameFilter(),
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
	setFilenameFilter(setting("FilenameFilter", QString()).toString(), true);

	_initialized = true;
}

void ImageCollectionScanner::setImageLoaderPlugin(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;
}

QString ImageCollectionScanner::getDirectory() const
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

bool ImageCollectionScanner::getSeparateByDirectory() const
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

QStringList ImageCollectionScanner::getSupportedImageTypes() const
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

QString ImageCollectionScanner::getFilenameFilter() const
{
	return _filenameFilter;
}

void ImageCollectionScanner::setFilenameFilter(const QString& filenameFilter, const bool& notify /*= false*/)
{
	if (!notify && filenameFilter == _filenameFilter)
		return;

	_filenameFilter = filenameFilter;

	setSetting("FilenameFilter", _filenameFilter);

	qDebug() << "Set filename filter" << _filenameFilter;

	emit filenameFilterChanged(_filenameFilter);
}

void ImageCollectionScanner::scan()
{
	try
	{
		std::vector<ImageCollection*> imageCollections;

		QStringList nameFilters;

		for (const auto& supportedImageType : getSupportedImageTypes())
			nameFilters << "*." + supportedImageType;

		scanDir(_directory, nameFilters, imageCollections, true);

		qDebug() << "Found " << imageCollections.size() << "image collections";

		for (auto& imageCollection : imageCollections) {
			imageCollection->computeDatasetName();
		}

		auto& imageCollectionsModel = _imageLoaderPlugin->getImageCollectionsModel();

		imageCollectionsModel.clear();
		imageCollectionsModel.insert(0, imageCollections);
	}
	catch (const std::runtime_error& e)
	{
		QMessageBox::critical(nullptr, QString("Unable to scan %1").arg(_directory), e.what());
	}
	catch (std::exception e)
	{
		QMessageBox::critical(nullptr, QString("Unable to scan %1").arg(_directory), e.what());
	}
}

auto ImageCollectionScanner::findImageCollection(std::vector<ImageCollection*>& imageCollections, const QString& directory, const QString& imageType, const QSize& imageSize)
{
	return std::find_if(imageCollections.begin(), imageCollections.end(), [this, &directory, &imageType, &imageSize](const auto& imageCollection) {
		if (_separateByDirectory && imageCollection->getDirectory(Qt::EditRole).toString() != directory)
			return false;

		if (imageCollection->getImageType(Qt::EditRole).toString() != imageType)
			return false;

		if (imageCollection->getSourceSize(Qt::EditRole).toSize() != imageSize)
			return false;

		return true;
	});
}

void ImageCollectionScanner::scanDir(const QString& directory, QStringList nameFilters, std::vector<ImageCollection*>& imageCollections, const bool& showProgressDialog /*= false*/)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	QProgressDialog* progressDialog = nullptr;

	if (showProgressDialog && !dirList.isEmpty()) {
		progressDialog = new QProgressDialog("Scanning", "Abort scanning", 0, dirList.size(), nullptr);

		progressDialog->setWindowTitle(QString("Scanning %1 for image collections").arg(directory));
		progressDialog->setWindowModality(Qt::WindowModal);
		progressDialog->setMinimumDuration(500);
		progressDialog->setValue(0);
		progressDialog->setFixedWidth(600);
	}
	
	for (int dirIndex = 0; dirIndex < dirList.size(); ++dirIndex)
	{
		if (progressDialog && progressDialog->wasCanceled())
			break;

		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(dirIndex));

		if (progressDialog) {
			progressDialog->setLabelText(QString("Scanning %1 for images").arg(path));
			
		}

		scanDir(path, nameFilters, imageCollections);

		QCoreApplication::processEvents();

		if (progressDialog)
			progressDialog->setValue(dirIndex);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto fileName			= fileList.at(i);
		const auto imageFilePath	= QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);
		const auto rootDir			= QFileInfo(imageFilePath).absoluteDir().path();

		QImageReader imageReader(imageFilePath);

		auto imageType	= QFileInfo(fileName).suffix().toUpper();
		auto pageCount	= 0;

		if (imageType == "TIFF") {
			imageReader.jumpToNextImage();

			pageCount = imageReader.imageCount();

			if (pageCount > 1) {
				imageType = "TIFF (multipage)";
			}
		}

		const auto imageSize = imageReader.size();

		auto it = findImageCollection(imageCollections, rootDir, imageType, imageSize);

		if (it == imageCollections.end()) {
			auto imageCollection = new ImageCollection(_imageLoaderPlugin->getImageCollectionsModel().rootItem(), rootDir, imageType, imageReader.imageFormat(), imageSize);

			if (imageType == "TIFF (multipage)") {
				for (int pageIndex = 0; pageIndex < pageCount; pageIndex++) {
					imageCollection->addImage(imageFilePath, pageIndex);
				}
			}
			else {
				imageCollection->addImage(imageFilePath);
			}

			imageCollections.push_back(imageCollection);
		}
		else {
			(*it)->addImage(imageFilePath);
		}
	}

	if (progressDialog != nullptr)
		delete progressDialog;
}