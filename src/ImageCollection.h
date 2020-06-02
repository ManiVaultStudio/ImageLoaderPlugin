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
		 * @param name Name of the image
		 * @param shouldLoad Whether the image should be loaded
		 * @param filePath Absolute file path of the image
		 * @param sourceSize Source size of the image
		 */
		Image(const QString& name, const bool& shouldLoad, const QString& filePath, const QSize& sourceSize);

	public: // Getters/setters

		/** Returns the parent image collection */
		ImageCollection* imageCollection();

		/**
		 * Sets the parent image collection
		 * @param imageCollection Parent image collection
		 */
		void setImageCollection(ImageCollection* imageCollection);

		/** Returns the image name */
		QString name() const;

		/**
		 * Sets the image name
		 * @param name Image name
		 */
		void setName(const QString& name);

		/** Returns whether the image should be loaded or not */
		bool shouldLoad() const;

		/**
		 * Sets whether the image should be loaded or not
		 * @param shouldLoad Whether the image should be loaded
		 */
		void setShouldLoad(const bool& shouldLoad);

		/** Returns the absolute image file path */
		QString filePath() const;

		/**
		 * Sets the absolute image file path
		 * @param filePath Absolute image file path
		 */
		void setShouldLoad(const QString& filePath);

		/** Returns the source image size */
		QSize sourceSize() const;

		/**
		 * Sets the source image size
		 * @param sourceSize Size of the source image
		 */
		void setSourceSize(const QSize& sourceSize);

		/** Returns the target image size */
		QSize targetSize() const;

		/**
		 * Sets the target image size
		 * @param targetSize Size of the target image
		 */
		void setTargetSize(const QSize& targetSize);

	private:
		ImageCollection*	_imageCollection;	/** Pointer to the image collection */
		QString				_name;				/** The dimension name of the image */
		bool				_shouldLoad;		/** Whether the image should be loaded */
		QString				_filePath;			/** The absolute image file path */
		QSize				_sourceSize;		/** Size of the source image */
		QSize				_targetSize;		/** Size of the target image */
	};

public: // Construction

	/** Default constructor */
	ImageCollection();

public: // Getters/setters

	/** Returns the image collection name */
	QString name() const;

	/**
	 * Sets the image collection name
	 * @param name Image collection name
	 */
	void setName(const QString& name);

	/** Returns the search directory */
	QString searchDir() const;

	/**
	 * Sets the search directory
	 * @param searchDir Search directory
	 */
	void setSearchDir(const QString& searchDir);

	/** Returns the absolute file path */
	QString filePath() const;

	/**
	 * Sets the absolute file path
	 * @param filePath Absolute file path
	 */
	void setFilePath(const QString& filePath);

	/** Returns the source image size */
	QSize sourceSize() const;

	/**
	 * Sets the source image size
	 * @param sourceSize Size of the source image
	 */
	void setSourceSize(const QSize& sourceSize);

	/** Returns the target image size */
	QSize targetSize() const;

	/**
	 * Sets the target image size
	 * @param targetSize Size of the target image
	 */
	void setTargetSize(const QSize& targetSize);

	/** Returns the number of images */
	std::uint32_t noImages() const;

	/**
	 * Get image by index
	 * @param  index Image index
	 */
	Image* image(const std::uint32_t& index);

public:

	/**
	 * Add image to the collection
	 * @param image Image to add
	 */
	void add(const Image& image);

private:
	QString				_name;				/** The name of the image collection */
	QString				_searchDir;			/** Initial directory where the search was started */
	QString				_filePath;			/** Absolute file path (multi-layer TIFF) */
	QSize				_sourceSize;		/** Size of the source image */
	QSize				_targetSize;		/** Size of the target image */
	QVector<Image>		_images;			/** Images */
};