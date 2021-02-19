#pragma once

#include "Common.h"
#include "TreeItem.h"

#include "ImageData/Images.h"

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
    enum class Column {
        DatasetName,                /** The name of the dataset */
        FileNames,                  /** The filename(s) */
        ImageType,                  /** The type of image(s) */
        ImageFormat,                /** Image format */
        ToGrayscale,                /** Whether to convert the images to grayscale */
        NoImages,                   /** Number of images in the collection */
        NoSelectedImages,           /** Number of selected images in the collection */
        DimensionTag,               /** Dimension TIFF tag */
        SourceSize,                 /** Size of the source image(s) */
        TargetSize,                 /** Size of the target image(s) */
        TargetWidth,                /** Target width of the image(s) */
        TargetHeight,               /** Target height of the image(s) */
        Type,                       /** Load as image sequence (0) or image stack (1) */
        SubsamplingEnabled,         /** Whether subsampling is enabled */
        SubsamplingRatio,           /** Subsampling ratio */
        SubsamplingFilter,          /** Subsampling filter */
        NoPoints,                   /** Number of high-dimensional data points */
        NoDimensions,               /** Number of high-dimensional data dimensions */
        Memory,                     /** Estimated memory consumption by the high-dimensional data */
        Directory,                  /** Directory */

        Start = DatasetName,        /** Column start */
        End = Directory             /** Column End */
    };

    /** Image collection bit flags */
    enum class Flag {
        DimensionNamesGuessed = 0x01,       /** Whether the dimension names have already been guessed */
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
        void guessDimensionName();

        /** Get parent image collection */
        ImageCollection* getImageCollection();

    private:
        std::int32_t    _index;             /** Image index (index < 0: image not loaded)*/
        QString         _filePath;          /** The absolute image file path */
        QString         _fileName;          /** The file name */
        QString         _dimensionName;     /** Dimension name (in case of image stack) */
        bool            _shouldLoad;        /** Whether the image should be loaded */
        std::int32_t    _pageIndex;         /** Page index (in case of multi-page TIFF) */
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

        /**
         * Image resampling filter
         * Defines image resampling filters for image subsampling
         */
        enum class ImageResamplingFilter
        {
            Box,            /** Box filter */
            Bilinear,       /** Bilinear filter */
            BSpline,        /** B-spline filter */
            Bicubic,        /** B-cubic filter */
            CatmullRom,     /** Catmull-rom filter */
            Lanczos         /** Lanczos filter */
        };

        /** Get string representation of image resampling filter enumeration */
        static QString imageResamplingFilterName(const ImageResamplingFilter& imageResamplingFilter) {
            switch (imageResamplingFilter)
            {
                case ImageResamplingFilter::Box:
                    return "Box";

                case ImageResamplingFilter::Bilinear:
                    return "Bilinear";

                case ImageResamplingFilter::BSpline:
                    return "BSpline";

                case ImageResamplingFilter::Bicubic:
                    return "Bicubic";

                case ImageResamplingFilter::CatmullRom:
                    return "CatmullRom";

                case ImageResamplingFilter::Lanczos:
                    return "Lanczos";
            }

            return "";
        }

    public: // Construction/destruction

        /**
         * Constructor
         * @param imageCollection Image collection
         * @param enabled Whether subsampling is enabled
         * @param ratio The subsampling ratio
         * @param filter The subsampling filter
         */
        SubSampling(ImageCollection* imageCollection, const bool& enabled = false, const float& ratio = 0.5f, const ImageResamplingFilter& filter = ImageResamplingFilter::Bicubic);

    public: // Getters/setters

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
        void setFilter(const ImageResamplingFilter& filter);

    private:
        ImageCollection*        _imageCollection;   /** Parent image collection */
        bool                    _enabled;           /** Whether subsampling is enabled */
        float                   _ratio;             /** Subsampling ratio */
        ImageResamplingFilter   _filter;            /** Subsampling filter e.g. box, bilinear */
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
    void guessDimensionNames();

    /**
     * Loads the image collection into a high-dimensional data vector
     * @param imageLoaderPlugin Pointer to image loader plugin
     */
    bool load(ImageLoaderPlugin* imageLoaderPlugin);

private:

    /** Performs a sanity check on the loaded data (checks for NaN's) */
    bool containsNans(std::vector<float>& data);

protected:
    QString             _directory;         /** Root directory of the images */
    QString             _imageFileType;     /** Image file type */
    QImage::Format      _imageFormat;       /** Image format */
    QSize               _sourceSize;        /** Size of the source image */
    QSize               _targetSize;        /** Size of the target image */
    QString             _datasetName;       /** The name of the dataset */
    QString             _dimensionTag;      /** The dimension (TIFF) tag */
    bool                _toGrayscale;       /** Whether to convert the images in the collection to grayscale */
    ImageData::Type     _type;              /** How to load the collection (as image sequence or image stack) */
    SubSampling         _subsampling;       /** Subsampling parameters */

    friend class SubSampling;
};