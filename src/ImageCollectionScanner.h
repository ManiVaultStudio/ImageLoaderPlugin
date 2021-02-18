#pragma once

#include <QObject>

class ImageLoaderPlugin;
class ImageCollection;

/**
 * Image scanner class
 *
 * Provides functionality for (recursively) scanning a directory
 * for candidate image collections e.g. sequence and stack
 *
 * @author Thomas Kroes
 */
class ImageCollectionScanner : public QObject {
    Q_OBJECT

public:
    /** Default constructor */
    ImageCollectionScanner();

    /** Load image scanner settings */
    virtual void loadSettings();

    /**
     * Sets the image loader plugin
     * @param imageLoaderPlugin Pointer to image loader plugin (for interfacing with data models)
     */
    void setImageLoaderPlugin(ImageLoaderPlugin* imageLoaderPlugin);

    /** Returns the search directory */
    QString	getDirectory() const;

    /**
     * Sets the search directory
     * @param directory Search directory
     * @param notify Whether to notify others of this change
     */
    void setDirectory(const QString& directory, const bool& notify = false);

    /** Returns the separate by directory option */
    bool getSeparateByDirectory() const;

    /**
     * Sets the separate by directory option
     * @param separateByDirectory Separate by directory option
     * @param notify Whether to notify others of this change
     */
    void setSeparateByDirectory(const bool& separateByDirectory, const bool& notify = false);

    /** Returns the support image types */
    QStringList	getSupportedImageTypes() const;

    /**
     * Sets the supported image types
     * @param imageTypes Types of images to search for
     * @param notify Whether to notify others of this change
     */
    void setSupportedImageTypes(const QStringList& supportedImageTypes, const bool& notify = false);

    /** Returns the filename filter */
    QString	getFilenameFilter() const;

    /**
     * Sets the filename filter
     * @param filenameFilter Filename filter
     * @param notify Whether to notify others of this change
     */
    void setFilenameFilter(const QString& filenameFilter, const bool& notify = false);

    /**
     * Finds an image collection based on image type and size
     * @param imageCollections Image collections
     * @param dir Images directory
     * @param imageType Type of image
     * @param imageSize Image size
     */
    auto findImageCollection(std::vector<ImageCollection*>& imageCollections, const QString& directory, const QString& imageType, const QSize& imageSize);

public:

    /** Scan for image collections */
    void scan();

private:

    /**
     * Scan directory recursively
     * @param directory Search directory
     * @param nameFilters Image file types to filter
     * @param imageCollections Scanned image collections
     * @param showProgressDialog Whether to show a progress dialog during scanning
     */
    void scanDir(const QString& directory, QStringList nameFilters, std::vector<ImageCollection*>& imageCollections, const bool& showProgressDialog = false);

signals:

    /** Signals that settings changed */
    void settingsChanged();

    /** Signals that scanning began */
    void beginScan();

    /**
     * Signals that the search directory changed
     * @param directory Search directory
     */
    void directoryChanged(const QString& directory);

    /**
     * Signals that the separate by directory option changed
     * @param separateByDirectory Separate by directory option
     */
    void separateByDirectoryChanged(const bool& separateByDirectory);

    /**
     * Signals that the supported image types changed
     * @param supportedImageTypes Supported image types
     */
    void supportedImageTypesChanged(const QStringList& supportedImageTypes);

    /**
     * Signals that the filename filter changed
     * @param supportedImageTypes Supported image types
     */
    void filenameFilterChanged(const QString& filenameFilter);

    /**
     * Broadcasts a message
     * @param message Message to broadcast
     */
    void message(const QString& message);

protected:
    ImageLoaderPlugin*      _imageLoaderPlugin;         /** Image loader plugin instance */
    QString                 _directory;                 /** Top directory to search in (recursively) */
    bool                    _separateByDirectory;       /** Separate image collections by directory */
    QStringList             _previousDirectories;       /** List of previously visited directories */
    QStringList             _supportedImageTypes;       /** List of supported image types e.g. .tiff, .jpg */
    QString                 _filenameFilter;            /** Filename filter string */
    bool                    _initialized;               /** Whether the scanner is initialized or not */
};