#pragma once

#include "Common.h"
#include "TreeItem.h"

#include <ImageData/Images.h>
#include <Dataset.h>
#include <ForegroundTask.h>

#include <QObject>
#include <QString>
#include <QSize>
#include <QVector>
#include <QImage>

namespace FI {
#include <FreeImage.h>
}

class ImageLoaderPlugin;

/**
 * Image collection class
 *
 * @author Thomas Kroes
*/
class ImageCollection : public TreeItem
{
public: // Enumerations

    /** Columns */
    enum Column {
        Name,                           /** The name of the image collection */
        DatasetName,                    /** The name of the dataset in the data hierarchy */
        FileNames,                      /** The filename(s) */
        ImageType,                      /** The type of image(s) */
        ImageFormat,                    /** Image format */
        ToGrayscale,                    /** Whether to convert the images to grayscale */
        NoImages,                       /** Number of images in the collection */
        NoSelectedImages,               /** Number of selected images in the collection */
        IsMultiPage,                    /** Whether the collection is a multi-page (TIFF) file */
        DimensionTag,                   /** Dimension TIFF tag */
        SourceSize,                     /** Size of the source image(s) */
        TargetSize,                     /** Size of the target image(s) */
        TargetWidth,                    /** Target width of the image(s) */
        TargetHeight,                   /** Target height of the image(s) */
        Type,                           /** Load as image sequence (0) or image stack (1) */
        SubsamplingType,                /** Type of subsampling (immediate/pyramid) */
        SubsamplingRatio,               /** Subsampling ratio */
        SubsamplingFilter,              /** Subsampling filter */
        SubsamplingNumberOfLevels,      /** Subsampling number of levels (in case of image pyramids) */
        SubsamplingLevelFactor,         /** Subsampling level factor (in case of image pyramids) */
        NoPoints,                       /** Number of high-dimensional data points */
        NoDimensions,                   /** Number of high-dimensional data dimensions */
        Memory,                         /** Estimated memory consumption by the high-dimensional data */
        Directory,                      /** Directory */
        Conversion,                     /** Conversion */

        Start = Name,                   /** Column start */
        End = Conversion                /** Column End */
    };

public: // Nested image class

    /**
     * Image class
     *
     * This image tree item is part of an image collection and is responsible for loading pixels into a high-dimensional data vector
     *
     * @author Thomas Kroes
     */
    class Image : public TreeItem
    {
    public: // Enumerations

        /** Image model columns */
        enum class Column {
            ShouldLoad = ult(ImageCollection::Column::End) + 1,     /** Whether to load the image or not */
            FileName,                                               /** The filename of the dataset */
            DimensionName,                                          /** Dimension name (in case of image stack) */
            FilePath,                                               /** Number of images in the collection */

            Start = ShouldLoad,                                     /** Column start */
            End = FilePath                                          /** Column End */
        };

    public: // Construction

        /**
         * Constructor
         * @param parent Parent tree item
         * @param filePath Absolute file path of the image
         * @param pageIndex Page index (in case of multi-page TIFF)
         */
        Image(TreeItem* parent, const QString& filePath, const std::int32_t& pageIndex = -1);

    public: // Getters/setters

        /**
         * Returns whether the image should be loaded or not
         * @param role Data role
         * @return Whether the image should be loaded or not in variant form
         */
        QVariant getShouldLoad(const int& role) const;

        /**
         * Sets whether the image should be loaded or not
         * @param shouldLoad Whether the image should be loaded
         */
        void setShouldLoad(const bool& shouldLoad);

        /**
         * Returns the image filename
         * @param role Data role
         * @return Image filename in variant form
         */
        QVariant getFileName(const int& role) const;

        /**
         * Returns the dimension name (in case of image stack)
         * @param role Data role
         * @return The dimension name in variant form
         */
        QVariant getDimensionName(const int& role) const;

        /**
         * Sets the dimension name (in case of image stack)
         * @param dimensionName Dimension name
         */
        void setDimensionName(const QString& dimensionName);

        /**
         * Returns the absolute image file path
         * @param role Data role
         * @return The absolute image file path in variant form
         */
        QVariant getFilePath(const int& role) const;

        /**
         * Sets the absolute image file path
         * @param filePath Absolute image file path
         */
        void setFilePath(const QString& filePath);

        /**
         * Returns the multi-page TIFF page index
         * @param role Data role
         * @return The page index in variant form
         */
        QVariant getPageIndex(const int& role) const;

        /**
         * Sets the page index
         * @param pageIndex Page index
         */
        void setPageIndex(const std::int32_t& pageIndex);

    public:

        /**
         * Loads the image into a high-dimensional data vector
         * @param imageLoaderPlugin Pointer to image loader plugin
         * @param data High-dimensional data vector
         * @param imageIndex Image index
         * @param dimensionNames Dimension names
         * @param multiBitmap Multi-bitmap in case of multi-page TIFF
         */
        void load(ImageLoaderPlugin* imageLoaderPlugin, std::vector<float>& data, const std::uint32_t& imageIndex, QStringList& dimensionNames, FI::FIMULTIBITMAP* multiBitmap = nullptr);

        /**
         * Loads the image bitmap into a high-dimensional data vector
         * @param bitmap Handle to FreeImage bitmap
         * @param data High-dimensional data vector
         * @param imageIndex Image index
         * @param dimensionNames Dimension names
         */
        void loadBitmap(FI::FIBITMAP* bitmap, std::vector<float>& data, const std::uint32_t& imageIndex, QStringList& dimensionNames);

        /** Guesses dimension name of the image */
        QString guessDimensionName();

        /** Get parent image collection */
        ImageCollection* getImageCollection();

    public: // Operators

        void operator = (const Image& other) {
            _index          = other._index;
            _filePath       = other._filePath;
            _fileName       = other._fileName;
            _dimensionName  = other._dimensionName;
            _shouldLoad     = other._shouldLoad;
            _pageIndex      = other._pageIndex;
        }

    private:
        std::int32_t    _index;                         /** Image index (index < 0: image not loaded)*/
        QString         _filePath;                      /** The absolute image file path */
        QString         _fileName;                      /** The file name */
        QString         _dimensionName;                 /** Dimension name (in case of image stack) */
        bool            _shouldLoad;                    /** Whether the image should be loaded */
        std::int32_t    _pageIndex;                     /** Page index (in case of multi-page TIFF) */
    };

    /**
     * Subsampling class
     *
     * Class for storing image subsampling parameters
     *
     * @author Thomas Kroes
     */
    class SubSampling
    {
    public: // Enumerations

        /** Image subsampling type */
        enum class Type
        {
            None,       /** No image subsampling, loading as-is */
            Resample,   /** Resample */
            Pyramid     /** Build an image pyramid and load it in the data model */
        };

        static const QMap<Type, QString> types;

        /** Image subsampling filter types for immediate subsampling */
        enum class Filter
        {
            Box,            /** Box filter */
            Bilinear,       /** Bilinear filter */
            BSpline,        /** B-spline filter */
            Bicubic,        /** B-cubic filter */
            CatmullRom,     /** Catmull-rom filter */
            Lanczos         /** Lanczos filter */
        };

        static const QMap<Filter, QString> filters;

    public: // Construction/destruction

        /**
         * Constructor
         * @param imageCollection Image collection
         * @param enabled Whether subsampling is enabled
         * @param ratio The subsampling ratio
         * @param filter The subsampling filter
         */
        SubSampling(ImageCollection* imageCollection, const bool& enabled = false, const float& ratio = 0.5f, const Filter& filter = Filter::Bicubic);

    public: // Getters/setters

        /**
         * Returns subsampling type
         * @param role Data role
         * @return Subsampling type in variant form
         */
        QVariant getType(const int& role) const;

        /**
         * Sets subsampling type
         * @param type Subsampling type
         */
        void setType(const Type& type);

        /**
         * Returns whether subsampling is enabled
         * @param role Data role
         * @return Whether subsampling is enabled in variant form
         */
        QVariant getEnabled(const int& role) const;

        /**
         * Sets whether subsampling is enabled
         * @param enabled Whether subsampling is enabled
         */
        void setEnabled(const bool& enabled);

        /**
         * Returns the subsampling ratio
         * @param role Data role
         * @return The subsampling ratio in variant form
         */
        QVariant getRatio(const int& role) const;

        /**
         * Sets the subsampling ratio
         * @param enabled The subsampling ratio
         */
        void setRatio(const float& ratio);

        /**
         * Returns the subsampling filter
         * @param role Data role
         * @return The subsampling filter in variant form
         */
        QVariant getFilter(const int& role) const;

        /**
         * Sets the subsampling filter
         * @param filter The subsampling filter
         */
        void setFilter(const Filter& filter);

        /**
         * Returns the number of pyramid levels
         * @param role Data role
         * @return The number of pyramid levels in variant form
         */
        QVariant getNumberOfLevels(const int& role) const;

        /**
         * Sets the number of pyramid levels
         * @param numberOfLevels The number of pyramid levels
         */
        void setNumberOfLevels(std::uint32_t numberOfLevels);

        /**
         * Returns the level factor
         * @param role Data role
         * @return The level factor in variant form
         */
        QVariant getLevelFactor(const int& role) const;

        /**
         * Sets the level factor
         * @param levelFactor The level factor
         */
        void setLevelFactor(std::uint32_t levelFactor);

    public: // Operators

        void operator = (const SubSampling& other) {
            _imageCollection    = other._imageCollection;
            _type               = other._type;
            _ratio              = other._ratio;
            _filter             = other._filter;
            _numberOfLevels     = other._numberOfLevels;
            _levelFactor        = other._levelFactor;
        }

    private:
        ImageCollection*    _imageCollection;       /** Parent image collection */
        Type                _type;                  /** Type of subsampling (immediate/pyramid) */
        float               _ratio;                 /** Subsampling ratio */
        Filter              _filter;                /** Subsampling filter e.g. box, bilinear */
        std::uint32_t       _numberOfLevels;        /** Number of levels in case of image pyramids */
        std::uint32_t       _levelFactor;           /** Level factor in case of image pyramids */
    };

public: // Construction

    /**
     * Constructor
     * @param parent Parent tree item
     * @param directory Search directory
     * @param imageFormat Format of the image
     * @param sourceSize Source image size
     */
    ImageCollection(TreeItem* parent, const QString& directory, const QString& imageFileType, const QImage::Format& imageFormat, const QSize& sourceSize);

    /** Destructor */
    ~ImageCollection();

public: // Getters/setters

    /** Returns the root directory */
    QVariant getDirectory(const int& role) const;

    /**
     * Sets the root directory
     * @param rootDir Search directory
     */
    void setDirectory(const QString& directory);

    /** Returns the image type */
    QVariant getImageType(const int& role) const;

    /**
     * Sets the image type
     * @param imageType Image type
     */
    void setImageType(const QString& imageType);

    /**
     * Returns the image format
     * @param role Data role
     * @return Image format in variant form
     */
    QVariant getImageFormat(const int& role) const;

    /**
     * Sets the image format
     * @param type Image format
     */
    void setImageFormat(const QImage::Format& imageFormat);

    /** Returns whether to convert the images in the collection to grayscale */
    QVariant getToGrayscale(const int& role) const;

    /**
     * Sets whether to convert the images in the collection to grayscale
     * @param grayscale Whether to convert the images in the collection to grayscale
     */
    void setToGrayscale(const bool& grayscale);

    /** Returns the number of channels per pixel */
    QVariant getNumberOfChannelsPerPixel(const int& role) const;

    /** Returns the source image size */
    QVariant getSourceSize(const int& role) const;

    /**
     * Sets the source image size
     * @param sourceSize Size of the source image
     */
    void setSourceSize(const QSize& sourceSize);

    /** Returns the target image size */
    QVariant getTargetSize(const int& role) const;

    /** Returns the target image width */
    QVariant getTargetWidth(const int& role) const;

    /** Returns the target image height */
    QVariant getTargetheight(const int& role) const;

    /**
     * Sets the target image size
     * @param targetSize Size of the target image
     */
    void setTargetSize(const QSize& targetSize);

    /** Returns the image collection name */
    QVariant getName(const int& role) const;

    /**
     * Sets the image collection name
     * @param name Image collection name
     */
    void setName(const QString& name);

    /** Returns the dataset name */
    QVariant getDatasetName(const int& role) const;

    /**
     * Sets the dataset name
     * @param datasetName Dataset name
     */
    void setDatasetName(const QString& datasetName);

    /** Returns the file names */
    QVariant getFileNames(const int& role) const;

    /**
     * Returns the image collection type (image sequence or image stack)
     * @param role Data role
     * @return Image collection type in variant form
     */
    QVariant getType(const int& role) const;

    /**
     * Sets the image collection type (image sequence or stack)
     * @param type Image collection type
     */
    void setType(const ImageData::Type	& type);

    /**
     * Get image by index
     * @param index Image index
     * @return Pointer to image
     */
    Image* getImage(const std::uint32_t& index);

    /** Returns the number of images */
    QVariant getNoImages(const int& role) const;

    /** Returns the number of selected images */
    QVariant getNoSelectedImages(const int& role) const;

    /** Returns the dimension tag */
    QVariant getDimensionTag(const int& role) const;

    /**
     * Sets the dimension tag
     * @param dimensionTag Dimension tag
     */
    void setDimensionTag(const QString& dimensionTag);

    /** Returns whether the file is a multi-page (TIFF) file */
    QVariant getIsMultiPage(const int& role) const;

    /**
     * Returns the number of high-dimensional data points
     * @param role Data role
     */
    QVariant getNoPoints(const int& role) const;

    /**
     * Returns the number of high-dimensional data dimensions
     * @param role Data role
     */
    QVariant getNoDimensions(const int& role) const;

    /**
     * Returns the estimated memory consumption by the image collection
     * @param role Data role
     */
    QVariant getMemoryConsumption(const int& role) const;

    /** Get subsampling parameters */
    SubSampling& getSubsampling();

    /** Returns the root directory */
    QVariant getConversion(const int& role) const;

    /**
     * Sets the conversion
     * @param conversion Conversion
     */
    void setConversion(const QString& conversion);

    hdps::ModalTask& getTask();

public:

    /**
     * Add image to the collection
     * @param filePath Absolute file path of the image
     * @param pageIndex Page index (in case of multi-page TIFF)
     */
    void addImage(const QString& filePath, const std::int32_t& pageIndex = -1);

    /** Computes the dataset name based on the loaded images and their respective file paths */
    void computeDatasetName();

    /** Guesses dimension names */
    QStringList guessDimensionNames();

    /**
     * Loads the image collection into a high-dimensional data vector
     * @param imageLoaderPlugin Pointer to image loader plugin
     * @param parent Smart pointer to parent dataset (used for image pyramids)
     * @return Smart pointer to generated dataset (if loading was successful)
     * 
     */
    hdps::Dataset<hdps::DatasetImpl> load(ImageLoaderPlugin* imageLoaderPlugin, hdps::Dataset<hdps::DatasetImpl> parent = hdps::Dataset<hdps::DatasetImpl>());

public: // Operators

    void operator = (const ImageCollection& other) {
        _directory      = other._directory;
        _imageFileType  = other._imageFileType;
        _imageFormat    = other._imageFormat;
        _sourceSize     = other._sourceSize;
        _targetSize     = other._targetSize;
        _name           = other._name;
        _datasetName    = other._datasetName;
        _dimensionTag   = other._dimensionTag;
        _toGrayscale    = other._toGrayscale;
        _type           = other._type;
        _subsampling    = other._subsampling;
        _conversion     = other._conversion;
    }

private:

    /** Performs a sanity check on the loaded data (checks for NaN's) */
    bool containsNans(std::vector<float>& data);

protected:
    QString                 _directory;             /** Root directory of the images */
    QString                 _imageFileType;         /** Image file type */
    QImage::Format          _imageFormat;           /** Image format */
    QSize                   _sourceSize;            /** Size of the source image */
    QSize                   _targetSize;            /** Size of the target image */
    QString                 _name;                  /** The name of the image collection */
    QString                 _datasetName;           /** The name of the dataset */
    QString                 _dimensionTag;          /** The dimension (TIFF) tag */
    bool                    _toGrayscale;           /** Whether to convert the images in the collection to grayscale */
    ImageData::Type         _type;                  /** How to load the collection (as image sequence or image stack) */
    SubSampling             _subsampling;           /** Subsampling parameters */
    QString                 _conversion;            /** Conversion SHA */
    hdps::ModalTask         _task;                  /** Task for reporting load progress */

    friend class SubSampling;
};