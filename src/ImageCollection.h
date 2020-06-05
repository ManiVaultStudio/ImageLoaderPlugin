#pragma once

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

		/** Returns the absolute image file path */
		QString filePath() const;

		/**
		 * Sets the absolute image file path
		 * @param filePath Absolute image file path
		 */
		void setFilePath(const QString& filePath);

		/** Returns whether the image should be loaded or not */
		bool shouldLoad() const;

		/**
		 * Sets whether the image should be loaded or not
		 * @param shouldLoad Whether the image should be loaded
		 */
		void setShouldLoad(const bool& shouldLoad);

		/** Returns the page index (in case of multi-layer TIFF) */
		std::int32_t pageIndex() const;

		/**
		 * Sets the page index
		 * @param pageIndex Page index
		 */
		void setPageIndex(const std::int32_t& pageIndex);

	private:
		ImageCollection*	_imageCollection;	/** Pointer to the image collection */
		QString				_filePath;			/** The absolute image file path */
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
	QVariant grayscale(const int& role) const;

	/**
	 * Sets whether to convert the images in the collection to grayscale
	 * @param grayscale Whether to convert the images in the collection to grayscale
	 */
	void setGrayscale(const bool& grayscale);

	/** Returns the number of images */
	QVariant noImages(const int& role) const;

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

private:
	QString					_searchDir;			/** Initial directory where the search was started */
	QString					_imageType;			/** Type of image */
	QSize					_sourceSize;		/** Size of the source image */
	QSize					_targetSize;		/** Size of the target image */
	QString					_datasetName;		/** The name of the dataset */
	bool					_grayscale;			/** Whether to convert the images in the collection to grayscale */
	std::vector<Image>		_images;			/** Images */
};