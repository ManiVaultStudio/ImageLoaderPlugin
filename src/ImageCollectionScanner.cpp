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
	_directory(),
	_separateByDirectory(false),
	_previousDirectories(),
	_supportedImageTypes(),
	_filenameFilter(),
	_initialized(false)
{
}

void ImageCollectionScanner::loadSettings()
{
	const auto directory = _imageLoaderPlugin->getSetting("Scanner/Directory", "").toString();

	setDirectory(QDir(directory).exists() ? directory : "", true);
	setSeparateByDirectory(_imageLoaderPlugin->getSetting("Scanner/SeparateByDirectory", true).toBool());

	auto supportedImageTypes = QStringList();

	supportedImageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

	setSupportedImageTypes(supportedImageTypes);

	setSupportedImageTypes(_imageLoaderPlugin->getSetting("Scanner/ImageTypes", supportedImageTypes).toStringList(), true);
	setFilenameFilter(_imageLoaderPlugin->getSetting("Scanner/FilenameFilter", QString()).toString(), true);

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

	_imageLoaderPlugin->setSetting("Scanner/Directory", _directory);

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

	_imageLoaderPlugin->setSetting("Scanner/SeparateByDirectory", _separateByDirectory);

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

	_imageLoaderPlugin->setSetting("Scanner/ImageTypes", _supportedImageTypes);

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

	_imageLoaderPlugin->setSetting("Scanner/FilenameFilter", _filenameFilter);

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

		imageCollectionsModel.setPersistData(false);
		{
			imageCollectionsModel.insert(0, imageCollections);
		}
		imageCollectionsModel.setPersistData(true);
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

	QSharedPointer<QProgressDialog> progressDialog;

	if (showProgressDialog && !dirList.isEmpty()) {
		progressDialog = QSharedPointer<QProgressDialog>::create("Scanning", "Abort scanning", 0, dirList.size(), nullptr);

		progressDialog->setWindowTitle(QString("Scanning %1 for image collections").arg(directory));
		progressDialog->setWindowModality(Qt::WindowModal);
		progressDialog->setMinimumDuration(50000);
		progressDialog->setValue(0);
		progressDialog->setFixedWidth(600);
	}
	
    const auto hasProgressDialog = !progressDialog.isNull();

	for (int dirIndex = 0; dirIndex < dirList.size(); ++dirIndex)
	{
		if (hasProgressDialog && progressDialog->wasCanceled())
			break;

		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(dirIndex));

		if (hasProgressDialog)
			progressDialog->setLabelText(QString("Scanning %1 for images").arg(path));

		scanDir(path, nameFilters, imageCollections);

		QCoreApplication::processEvents();

		if (hasProgressDialog)
			progressDialog->setValue(dirIndex);
	}

    if (hasProgressDialog)
        progressDialog->reset();

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

		auto imageExtension = QFileInfo(fileName).suffix().toUpper();
		auto pageCount	    = 0;

        if (imageExtension == "TIFF" || imageExtension == "TIF") {
			imageReader.jumpToNextImage();

			pageCount = imageReader.imageCount();

			if (pageCount > 1) {
				imageExtension = "TIFF (multipage)";
			}
		}

		const auto imageSize = imageReader.size();

		auto it = findImageCollection(imageCollections, rootDir, imageExtension, imageSize);

		if (it == imageCollections.end()) {
			auto imageCollection = new ImageCollection(_imageLoaderPlugin->getImageCollectionsModel().rootItem(), rootDir, imageExtension, imageReader.imageFormat(), imageSize);

			if (imageExtension == "TIFF (multipage)") {
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
}