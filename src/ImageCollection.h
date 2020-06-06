#pragma once

#include "ImageData/Images.h"

#include <QObject>
#include <QString>
#include <QSize>
#include <QVector>

/**
 * Image collection class
 *
 * TODO: Write description
 *
 * @author Thomas Kroes
*/
class ImageCollection
{
public: // Nested image class

	/**
	 * Image class
	 *
	 * TODO: Write description
	 *
	 * @author Thomas Kroes
	 */
	class Image
	{
	public: // Construction

		/** Default constructor */
		Image();

		/**
		 * Constructor
		 * @param filePath Absolute file path of the image
		 * @param pageIndex Page index (in case of multi-layer TIFF)
		 */
		Image(const QString& filePath, const std::int32_t& pageIndex = -1);

	public: // Getters/setters

		/** Returns the parent image collection */
		ImageCollection* imageCollection();

		/**
		 * Sets the parent image collection
		 * @param imageCollection Parent image collection
		 */
		void setImageCollection(ImageCollection* imageCollection);

		/**
		 * Returns the image name
		 * @param role Data role
		 * @return Image name in variant form
		 */
		QVariant name(const int& role) const;

		/**
		 * Returns the absolute image file path
		 * @param role Data role
		 * @return The absolute image file path in variant form
		 */
		QVariant filePath(const int& role) const;

		/**
		 * Sets the absolute image file path
		 * @param filePath Absolute image file path
		 */
		void setFilePath(const QString& filePath);

		/**
		 * Returns whether the image should be loaded or not
		 * @param role Data role
		 * @return Whether the image should be loaded or not in variant form
		 */
		QVariant shouldLoad(const int& role) const;

		/**
		 * Sets whether the image should be loaded or not
		 * @param shouldLoad Whether the image should be loaded
		 */
		void setShouldLoad(const bool& shouldLoad);

		/**
		 * Returns the multi-layer TIFF page index
		 * @param role Data role
		 * @return The page index in variant form
		 */
		QVariant pageIndex(const int& role) const;

		/**
		 * Sets the page index
		 * @param pageIndex Page index
		 */
		void setPageIndex(const std::int32_t& pageIndex);

	private:
		ImageCollection*	_imageCollection;	/** Pointer to the image collection */
		QString				_filePath;			/** The absolute image file path */
		QString				_name;				/** The image name */
		bool				_shouldLoad;		/** Whether the image should be loaded */
		std::int32_t		_pageIndex;			/** Page index (in case of multi-layer TIFF) */
	};

public: // Construction

	/**
	 * Constructor
	 * @param searchDir Search directory
	 * @param imageType Image type
	 * @param sourceSize Source image size
	 */
	ImageCollection(const QString& searchDir, const QString& imageType, const QSize& sourceSize);

public: // Getters/setters

	/** Returns the search directory */
	QVariant searchDir(const int& role) const;

	/**
	 * Sets the search directory
	 * @param searchDir Search directory
	 */
	void setSearchDir(const QString& searchDir);

	/** Returns the image type */
	QVariant imageType(const int& role) const;

	/**
	 * Sets the image type
	 * @param imageType Image type
	 */
	void setImageType(const QString& imageType);

	/** Returns the source image size */
	QVariant sourceSize(const int& role) const;

	/**
	 * Sets the source image size
	 * @param sourceSize Size of the source image
	 */
	void setSourceSize(const QSize& sourceSize);

	/** Returns the target image size */
	QVariant targetSize(const int& role) const;

	/**
	 * Sets the target image size
	 * @param targetSize Size of the target image
	 */
	void setTargetSize(const QSize& targetSize);

	/** Returns the dataset name */
	QVariant datasetName(const int& role) const;

	/**
	 * Sets the dataset name
	 * @param datasetName Dataset name
	 */
	void setDatasetName(const QString& datasetName);

	/** Returns whether to convert the images in the collection to grayscale */
	QVariant toGrayscale(const int& role) const;

	/**
	 * Sets whether to convert the images in the collection to grayscale
	 * @param grayscale Whether to convert the images in the collection to grayscale
	 */
	void setToGrayscale(const bool& grayscale);

	/**
	 * Returns the image collection type (image sequence or image stack)
	 * @param role Data role
	 * @return Image collection type in variant form
	 */
	QVariant type(const int& role) const;

	/**
	 * Sets the image collection type (image sequence or stack)
	 * @param type Image collection type
	 */
	void setType(const ImageData::Type	& type);

	/** Returns the number of images */
	QVariant noImages(const int& role) const;

	/** Returns the number of selected images */
	QVariant noSelectedImages(const int& role) const;

	/** Get images */
	const std::vector<Image>& images() const;

	/**
	 * Get image by index
	 * @param  index Image index
	 */
	Image* image(const std::uint32_t& index);

public:

	/**
	 * Add image to the collection
	 * @param filePath Absolute file path of the image
	 * @param pageIndex Page index (in case of multi-layer TIFF)
	 */
	void addImage(const QString& filePath, const std::int32_t& pageIndex = -1);

	/** Computes the dataset name based on the loaded images and their respective file paths */
	void computeDatasetName();

private:
	QString					_searchDir;			/** Initial directory where the search was started */
	QString					_imageType;			/** Type of image */
	QSize					_sourceSize;		/** Size of the source image */
	QSize					_targetSize;		/** Size of the target image */
	QString					_datasetName;		/** The name of the dataset */
	bool					_toGrayscale;		/** Whether to convert the images in the collection to grayscale */
	ImageData::Type			_type;				/** How to load the collection (as image sequence or image stack) */
	std::vector<Image>		_images;			/** Images */
};