#include "ImageCollection.h"

#include <QDebug>
#include <QDir>

ImageCollection::Image::Image() :
	_imageCollection(nullptr),
	_filePath(),
	_shouldLoad(true),
	_pageIndex(-1)
{
}

ImageCollection::Image::Image(const QString& filePath, const std::int32_t& pageIndex /*= -1*/) :
	_imageCollection(nullptr),
	_shouldLoad(true),
	_filePath(filePath),
	_pageIndex(pageIndex)
{
}

ImageCollection* ImageCollection::Image::imageCollection()
{
	return _imageCollection;
}

void ImageCollection::Image::setImageCollection(ImageCollection* imageCollection)
{
	_imageCollection = imageCollection;
}

QString ImageCollection::Image::filePath() const
{
	return _filePath;
}

void ImageCollection::Image::setFilePath(const QString& filePath)
{
	_filePath = filePath;
}

bool ImageCollection::Image::shouldLoad() const
{
	return _shouldLoad;
}

void ImageCollection::Image::setShouldLoad(const bool& shouldLoad)
{
	_shouldLoad = shouldLoad;
}

std::int32_t ImageCollection::Image::pageIndex() const
{
	return _pageIndex;
}

void ImageCollection::Image::setPageIndex(const std::int32_t& pageIndex)
{
	_pageIndex = pageIndex;
}

ImageCollection::ImageCollection(const QString& searchDir, const QString& imageType, const QSize& sourceSize) :
	_searchDir(searchDir),
	_imageType(imageType),
	_sourceSize(sourceSize),
	_targetSize(sourceSize),
	_datasetName(),
	_images()
{
	_datasetName = QString("%1_%2_%3_%4").arg(QDir(searchDir).dirName(), imageType, QString::number(sourceSize.width()), QString::number(sourceSize.height()));
}

QVariant ImageCollection::searchDir(const int& role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return _searchDir;

		case Qt::EditRole:
			return _searchDir;

		case Qt::ToolTipRole:
			return QString("Search directory: %1").arg(_searchDir);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setSearchDir(const QString& searchDir)
{
	_searchDir = searchDir;
}

QVariant ImageCollection::imageType(const int& role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return _imageType;

		case Qt::EditRole:
			return _imageType;

		case Qt::ToolTipRole:
			return QString("Image type: %1").arg(_imageType);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setImageType(const QString& imageType)
{
	_imageType = imageType;
}

QVariant ImageCollection::sourceSize(const int& role) const
{
	const auto sourceSizeString = QString("%1x%2").arg(QString::number(_sourceSize.width()), QString::number(_sourceSize.height()));

	switch (role)
	{
		case Qt::DisplayRole:
			return sourceSizeString;

		case Qt::EditRole:
			return _sourceSize;

		case Qt::ToolTipRole:
			return QString("Source image size: %1").arg(sourceSizeString);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setSourceSize(const QSize& sourceSize)
{
	_sourceSize = sourceSize;
}

QVariant ImageCollection::targetSize(const int& role) const
{
	const auto targetSizeString = QString("%1x%2").arg(QString::number(_targetSize.width()), QString::number(_targetSize.height()));

	switch (role)
	{
		case Qt::DisplayRole:
			return targetSizeString;

		case Qt::EditRole:
			return _targetSize;

		case Qt::ToolTipRole:
			return QString("Source image size: %1").arg(targetSizeString);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setTargetSize(const QSize& targetSize)
{
	_targetSize = targetSize;
}

QVariant ImageCollection::datasetName(const int& role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return _datasetName;

		case Qt::EditRole:
			return _datasetName;

		case Qt::ToolTipRole:
			return QString("Dataset name: %1").arg(_datasetName);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setDatasetName(const QString& datasetName)
{
	_datasetName = datasetName;
}

QVariant ImageCollection::noImages(const int& role) const
{
	const auto noImagesString = QString::number(_images.size());

	switch (role)
	{
		case Qt::DisplayRole:
			return noImagesString;

		case Qt::EditRole:
			return _images.size();

		case Qt::ToolTipRole:
			return QString("Number of images: %1").arg(noImagesString);

		default:
			break;
	}

	return QVariant();
}

ImageCollection::Image* ImageCollection::image(const std::uint32_t& index)
{
	return &_images[index];
}

void ImageCollection::addImage(const QString& filePath, const std::int32_t& pageIndex /*= -1*/)
{
	_images.push_back(Image(filePath, pageIndex));

	_images.back().setImageCollection(this);
}