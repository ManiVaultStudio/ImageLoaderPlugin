#include "ImageCollectionScanner.h"
#include "ImageLoaderPlugin.h"
#include "ImageCollection.h"

#include <BackgroundTask.h>

#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QImageReader>
#include <QMessageBox>
#include <QStringList>
#include <QSysInfo>

#include <stdexcept>

namespace fi {
#include <FreeImage.h>
}

using namespace mv;

ImageCollectionScanner::ImageCollectionScanner(ImageLoaderPlugin& imageLoaderPlugin) :
    _imageLoaderPlugin(imageLoaderPlugin),
    _directory(),
    _separateByDirectory(false),
    _previousDirectories(),
    _supportedImageTypes(),
    _filenameFilter(),
    _initialized(false),
    _imageCollections(),
    _indexImageCollectionsTask(this, "Indexing image collections")
{
    _indexImageCollectionsTask.setDescription("Scanning disk for image collections");
    _indexImageCollectionsTask.setIcon(Application::getIconFont("FontAwesome").getIcon("barcode"));
    _indexImageCollectionsTask.setMayKill(true);
}
	
ImageCollectionScanner::~ImageCollectionScanner()
{
    for (auto imageCollection : _imageCollections)
        delete imageCollection;
}

void ImageCollectionScanner::loadSettings()
{
    const auto directory = _imageLoaderPlugin.getSetting("Scanner/Directory", QSysInfo::productType() == QString("macos") ? QDir::homePath() : "").toString();

    setDirectory(QDir(directory).exists() ? directory : QDir::homePath(), true);
    setSeparateByDirectory(_imageLoaderPlugin.getSetting("Scanner/SeparateByDirectory", true).toBool());

    auto supportedImageTypes = QStringList();	

    supportedImageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

    setSupportedImageTypes(supportedImageTypes);

    setSupportedImageTypes(_imageLoaderPlugin.getSetting("Scanner/ImageTypes", supportedImageTypes).toStringList(), true);
    setFilenameFilter(_imageLoaderPlugin.getSetting("Scanner/FilenameFilter", QString()).toString(), true);

    _initialized = true;
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

    _imageLoaderPlugin.setSetting("Scanner/Directory", _directory);

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

    _imageLoaderPlugin.setSetting("Scanner/SeparateByDirectory", _separateByDirectory);

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

    _imageLoaderPlugin.setSetting("Scanner/ImageTypes", _supportedImageTypes);

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

    _imageLoaderPlugin.setSetting("Scanner/FilenameFilter", _filenameFilter);

    qDebug() << "Set filename filter" << _filenameFilter;

    emit filenameFilterChanged(_filenameFilter);
}

void ImageCollectionScanner::scan()
{
    try
    {
        QVector<ImageCollection*> imageCollections;

        //for (auto imageCollection : _imageCollections)
        //    delete imageCollection;

        //_imageCollections.clear();

        QStringList nameFilters;

        for (const auto& supportedImageType : getSupportedImageTypes())
            nameFilters << "*." + supportedImageType;

        scanDir(_directory, nameFilters, imageCollections, true);
        if (_indexImageCollectionsTask.isAborting() || _indexImageCollectionsTask.isAborted()) {
            _indexImageCollectionsTask.setAborted();
            return;
        }

        qDebug() << "Found " << imageCollections.count() << "image collections";

        _indexImageCollectionsTask.setSubtaskStarted("Database", "Building database");

        QCoreApplication::processEvents();

        for (auto& imageCollection : imageCollections)
            imageCollection->computeDatasetName();

        auto& imageCollectionsModel = _imageLoaderPlugin.getImageCollectionsModel();

        imageCollectionsModel.clear();

        imageCollectionsModel.setPersistData(false);
        {
            imageCollectionsModel.insert(0, imageCollections);
        }
        imageCollectionsModel.setPersistData(true);

        QCoreApplication::processEvents();

        _indexImageCollectionsTask.setSubtaskFinished("Database", "Database built");
        _indexImageCollectionsTask.setFinished();
        
        QCoreApplication::processEvents();
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

auto ImageCollectionScanner::findImageCollection(QVector<ImageCollection*>& imageCollections, const QString& directory, const QString& imageType, const QSize& imageSize)
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

void ImageCollectionScanner::scanDir(const QString& directory, QStringList nameFilters, QVector<ImageCollection*>& imageCollections, const bool& showProgressDialog /*= false*/)
{
    if (_indexImageCollectionsTask.isAborting() || _indexImageCollectionsTask.isAborted())
        return;
    
    auto subDirectories = QDir(directory);

    subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    const auto dirList              = subDirectories.entryList();
    const auto hasProgressDialog    = showProgressDialog && !dirList.isEmpty();
    
    if (hasProgressDialog) {
        auto subTasks = dirList;
        
        subTasks << "TIFF" << "Database";

        _indexImageCollectionsTask.setSubtasks(subTasks);
        _indexImageCollectionsTask.setStatus(Task::Status::Running);
    }
    
    for (int dirIndex = 0; dirIndex < dirList.count(); ++dirIndex) {
        if (_indexImageCollectionsTask.isAborting())
            break;

        const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(dirIndex));

        if (hasProgressDialog)
            _indexImageCollectionsTask.setSubtaskStarted(dirList.at(dirIndex), QString("Scanning %1").arg(path));
        QCoreApplication::processEvents();
        scanDir(path, nameFilters, imageCollections);

        QCoreApplication::processEvents();

        if (hasProgressDialog)
            _indexImageCollectionsTask.setSubtaskFinished(dirList.at(dirIndex), QString("%1 scanned").arg(path));
    }

    if (_indexImageCollectionsTask.isAborting() || _indexImageCollectionsTask.isAborted()) {
        _indexImageCollectionsTask.setAborted();
        return;
    }
    
    auto imageFiles = QDir(directory);

    imageFiles.setFilter(QDir::Files);
    imageFiles.setNameFilters(nameFilters);

    if (hasProgressDialog) {
        _indexImageCollectionsTask.setSubtaskStarted("TIFF", "Indexing multi-page TIF image files");

        QCoreApplication::processEvents();
    }

    const auto fileList = imageFiles.entryList();

    for (int i = 0; i < fileList.count(); ++i) {
        if (_indexImageCollectionsTask.isAborting() || _indexImageCollectionsTask.isAborted()) {
            break;
        }
        const auto fileName         = fileList.at(i);
        const auto imageFilePath    = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);
        const auto rootDir          = QFileInfo(imageFilePath).absoluteDir().path();

        QImageReader imageReader(imageFilePath);

        if (!imageReader.canRead())
        {
            if (!imageReader.canRead())
            {
                qWarning() << "ImageLoaderPlugin: cannot read file " << fileName;

                QStringList supportedImageFormats;
                for (const QByteArray& byteArray : QImageReader::supportedImageFormats()) {
                    supportedImageFormats.append(QString(byteArray));  // Convert QByteArray to QString
                }

                qWarning() << "ImageLoaderPlugin: supported image formats: " << supportedImageFormats.join(", ");
                qWarning() << "ImageLoaderPlugin: consider installing the Qt Imaging Formats plugin to support formates like tiff and webp.";
            }
        }

        auto imageExtension = QFileInfo(fileName).suffix().toUpper();
        auto pageCount = 0;

        if (imageExtension == "TIFF" || imageExtension == "TIF") {
            imageReader.jumpToNextImage();

            pageCount = imageReader.imageCount();

            if (pageCount > 1)
                imageExtension = "TIFF (multipage)";
        }

        const auto imageSize = imageReader.size();

        auto it = findImageCollection(imageCollections, rootDir, imageExtension, imageSize);

        if (it == imageCollections.end() || imageExtension == "TIFF (multipage)") {
            auto imageCollection = new ImageCollection(_imageLoaderPlugin.getImageCollectionsModel().getRootItem(), rootDir, imageExtension, imageReader.imageFormat(), imageSize);

            if (imageExtension == "TIFF (multipage)") {
                for (int pageIndex = 0; pageIndex < pageCount; pageIndex++)
                    imageCollection->addImage(imageFilePath, pageIndex);
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
    if (_indexImageCollectionsTask.isAborting() || _indexImageCollectionsTask.isAborted()) {
        _indexImageCollectionsTask.setAborted();
        return;
    }

    if (hasProgressDialog) {
        _indexImageCollectionsTask.setSubtaskFinished("TIFF", "Multi-page TIF image files indexed");

        QCoreApplication::processEvents();
    }
}
