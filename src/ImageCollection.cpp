#include "ImageCollection.h"
#include "ImageLoaderPlugin.h"
#include "Common.h"

#include "PointData.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <algorithm>

namespace fi {
	#include <FreeImage.h>
}

ImageCollection::Image::Image(TreeItem* parent, const QString& filePath, const std::int32_t& pageIndex /*= -1*/) :
	TreeItem(parent),
	_index(-1),
	_filePath(filePath),
	_fileName(QFileInfo(filePath).completeBaseName()),
	_dimensionName(),
	_shouldLoad(true),
	_pageIndex(pageIndex)
{
	_dimensionName = QFileInfo(filePath).completeBaseName();
}

QVariant ImageCollection::Image::shouldLoad(const int& role) const
{
	const auto shouldLoadString = _shouldLoad ? "true" : "false";

	switch (role)
	{
		case Qt::DisplayRole:
			return "";

		case Qt::EditRole:
			return _shouldLoad;

		case Qt::ToolTipRole:
			return QString("Should load: %1").arg(shouldLoadString);

		case Qt::CheckStateRole:
			return _shouldLoad ? Qt::Checked : Qt::Unchecked;

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::Image::setShouldLoad(const bool& shouldLoad)
{
	_shouldLoad = shouldLoad;
}

QVariant ImageCollection::Image::fileName(const int& role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return _fileName;

		case Qt::EditRole:
			return _fileName;

		case Qt::ToolTipRole:
			return QString("File: %1").arg(_filePath);

		default:
			break;
	}

	return QVariant();
}

QVariant ImageCollection::Image::dimensionName(const int& role) const
{
	/*
	auto* pageBitmap = FreeImage_LockPage(multiBitmap, pageIndex);

	fi::FITAG* tag;

	fi::FreeImage_GetMetadata(fi::FREE_IMAGE_MDMODEL::FIMD_CUSTOM, pageBitmap, "DESCRIPTION", &tag);
	
	if (_dimensionName.isEmpty()) {
		_dimensionName = QFileInfo(_filePath).completeBaseName();

		if (_pageIndex >= 0)
			_dimensionName = QString("Dim %1").arg(QString::number(_pageIndex));
	}
	*/

	switch (role)
	{
		case Qt::DisplayRole:
			return _dimensionName;

		case Qt::EditRole:
			return _dimensionName;

		case Qt::ToolTipRole:
			return QString("Dimension name: %1").arg(_dimensionName);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::Image::setDimensionName(const QString& dimensionName)
{
	_dimensionName = dimensionName;
}

QVariant ImageCollection::Image::filePath(const int& role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return _filePath;

		case Qt::EditRole:
			return _filePath;

		case Qt::ToolTipRole:
			return QString("File path: %1").arg(_filePath);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::Image::setFilePath(const QString& filePath)
{
	_filePath = filePath;
}

QVariant ImageCollection::Image::pageIndex(const int& role) const
{
	const auto pageIndexString = QString::number(_pageIndex);

	switch (role)
	{
		case Qt::DisplayRole:
			return pageIndexString;

		case Qt::EditRole:
			return _pageIndex;

		case Qt::ToolTipRole:
			return QString("Page index: %1").arg(pageIndexString);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::Image::setPageIndex(const std::int32_t& pageIndex)
{
	_pageIndex = pageIndex;
}

void ImageCollection::Image::load(ImageLoaderPlugin* imageLoaderPlugin, std::vector<float>& data, const std::uint32_t& index)
{
	qDebug() << QString("Loading image: %1").arg(_fileName);

	if (_pageIndex >= 0)
	{
		auto* multiBitmap = FreeImage::freeImageOpenMultiBitmap(_filePath);

		if (multiBitmap != nullptr) {
			const auto noPages = FreeImage_GetPageCount(multiBitmap);

			for (int pageIndex = 0; pageIndex < noPages; pageIndex++) {
				auto* pageBitmap = FreeImage_LockPage(multiBitmap, pageIndex);

				if (pageBitmap != nullptr) {
					loadBitmap(pageBitmap, data, index);
					FreeImage_UnlockPage(multiBitmap, pageBitmap, false);
				}
			}

			FreeImage_CloseMultiBitmap(multiBitmap);
		}
	}
	else {
		auto* bitmap = FreeImage::freeImageLoad(_filePath);

		if (bitmap == nullptr)
			throw std::runtime_error("Unable to load image");

		loadBitmap(bitmap, data, index);

		fi::FreeImage_Unload(bitmap);
	}
}

template<class T> static void readSequence(fi::FIBITMAP* bitmap, const QSize& targetSize, std::vector<float>& data, const std::uint32_t& imageIndex, const std::int32_t& noComponents = 1)
{
	if (bitmap == nullptr)
		throw std::runtime_error("Bitmap handle is NULL");

	const auto targetWidth	= targetSize.width();
	const auto targetHeight	= targetSize.height();
	const auto noPixels		= targetWidth * targetHeight;

	for (std::int32_t y = 0; y < targetHeight; y++) {
		auto scanLine = reinterpret_cast<T*>(fi::FreeImage_GetScanLine(bitmap, y));

		if (scanLine == nullptr)
			throw std::runtime_error("Unable to read scan line");

		for (std::int32_t x = 0; x < targetWidth; x++) {
			const auto pixelIndex = y * targetWidth + x;

			if (noComponents == 1)
				data[imageIndex * noPixels + pixelIndex] = scanLine[x];

			if (noComponents >= 3) {
				auto sum = 0.0f;

				for (std::int32_t c = 0; c < 3; c++)
					sum += static_cast<float>(scanLine[x * noComponents + c]);

				data[imageIndex * noPixels + pixelIndex] = sum / 3.0f;
			}
		}
	}
}

template<class T>
static void readStack(fi::FIBITMAP* bitmap, const QSize& targetSize, std::vector<float>& data, const std::uint32_t& imageIndex, const std::uint32_t& noDimensions, const std::int32_t& noComponents = 1)
{
	if (bitmap == nullptr)
		throw std::runtime_error("Bitmap handle is NULL");

	const auto targetWidth	= targetSize.width();
	const auto targetHeight	= targetSize.height();
	const auto noPixels		= targetWidth * targetHeight;

	for (std::int32_t y = 0; y < targetHeight; y++) {
		auto scanLine = reinterpret_cast<T*>(fi::FreeImage_GetScanLine(bitmap, y));

		if (scanLine == nullptr)
			throw std::runtime_error("Unable to read scan line");

		for (std::int32_t x = 0; x < targetWidth; x++) {
			const auto pixelIndex = y * targetWidth + x;

			if (noComponents == 1)
				data[pixelIndex * noDimensions + imageIndex] = scanLine[x];

			if (noComponents >= 3) {
				auto sum = 0.0f;

				for (std::int32_t c = 0; c < 3; c++)
					sum += static_cast<float>(scanLine[x * noComponents + c]);

				data[pixelIndex * noDimensions + imageIndex] = sum / 3.0f;
			}
		}
	}
}

void ImageCollection::Image::loadBitmap(fi::FIBITMAP* bitmap, std::vector<float>& data, const std::uint32_t& imageIndex)
{
	fi::FIBITMAP* subsampledBitmap	= nullptr;
	fi::FIBITMAP* convertedBitmap	= nullptr;

	const auto handleException = [&](const QString& message) {
		if (subsampledBitmap != bitmap)
			fi::FreeImage_Unload(subsampledBitmap);

		fi::FreeImage_Unload(convertedBitmap);

		throw std::runtime_error(QString("Error loading %1: %2").arg(_fileName, message).toLatin1());
	};

	try {
		if (bitmap == nullptr)
			throw std::runtime_error("FreeImage bitmap handle is NULL");

		const auto imageCollection		= static_cast<ImageCollection*>(parentItem());
		const auto imageCollectionType	= static_cast<ImageData::Type>(imageCollection->type(Qt::EditRole).toInt());
		const auto sourceWidth			= fi::FreeImage_GetWidth(bitmap);
		const auto sourceHeight			= fi::FreeImage_GetHeight(bitmap);
		const auto targetSize			= imageCollection->targetSize(Qt::EditRole).toSize();
		const auto targetWidth			= targetSize.width();
		const auto targetHeight			= targetSize.height();
		const auto noDimensions			= imageCollection->noDimensions(Qt::EditRole).toInt();
		const auto subsample			= QSize(sourceWidth, sourceHeight) != targetSize;
		const auto filter				= static_cast<fi::FREE_IMAGE_FILTER>(imageCollection->subsampling().filter(Qt::EditRole).toInt());
		const auto grayScale			= imageCollection->toGrayscale(Qt::EditRole).toBool();

		subsampledBitmap = subsample ? fi::FreeImage_Rescale(bitmap, targetWidth, targetHeight, filter) : bitmap;

		if (subsampledBitmap == nullptr)
			throw std::runtime_error("Unable to subsample bitmap");

		const auto imageType = fi::FreeImage_GetImageType(bitmap);

		switch (imageType)
		{
			case fi::FIT_UINT16:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::uint16_t>(subsampledBitmap, targetSize, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::uint16_t>(subsampledBitmap, targetSize, data, imageIndex, noDimensions);

				break;
			}

			case fi::FIT_INT16:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::int16_t>(subsampledBitmap, targetSize, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::int16_t>(subsampledBitmap, targetSize, data, imageIndex, noDimensions);

				break;
			}

			case fi::FIT_UINT32:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::uint32_t>(subsampledBitmap, targetSize, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::uint32_t>(subsampledBitmap, targetSize, data, imageIndex, noDimensions);

				break;
			}

			case fi::FIT_INT32:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::int32_t>(subsampledBitmap, targetSize, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::int32_t>(subsampledBitmap, targetSize, data, imageIndex, noDimensions);

				break;
			}

			case fi::FIT_FLOAT:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<float>(subsampledBitmap, targetSize, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<float>(subsampledBitmap, targetSize, data, imageIndex, noDimensions);

				break;
			}
			
			case fi::FIT_DOUBLE:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<double>(subsampledBitmap, targetSize, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<double>(subsampledBitmap, targetSize, data, imageIndex, noDimensions);

				break;
			}

			case fi::FIT_COMPLEX:
				break;

			case fi::FIT_BITMAP:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<fi::BYTE>(subsampledBitmap, targetSize, data, imageIndex, 3);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<fi::BYTE>(subsampledBitmap, targetSize, data, imageIndex, noDimensions, 3);

				break;
			}

			case fi::FIT_RGB16:
				break;

			case fi::FIT_RGBF:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<float>(subsampledBitmap, targetSize, data, imageIndex, 3);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<float>(subsampledBitmap, targetSize, data, imageIndex, noDimensions, 3);

				break;
			}

			case fi::FIT_RGBA16:
				break;

			case fi::FIT_RGBAF:
			{
				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<float>(subsampledBitmap, targetSize, data, imageIndex, 4);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<float>(subsampledBitmap, targetSize, data, imageIndex, noDimensions, 4);

				break;
			}

			default:
				break;
		}
		
		fi::FreeImage_Unload(convertedBitmap);

		if (subsampledBitmap != bitmap)
			fi::FreeImage_Unload(subsampledBitmap);
	}
	catch (const std::runtime_error& e)
	{
		handleException(e.what());
	}
	catch (std::exception e)
	{
		handleException(e.what());
	}
}

ImageCollection::SubSampling::SubSampling(const bool& enabled /*= false*/, const float& ratio /*= 0.5f*/, const ImageResamplingFilter& filter /*= ImageResamplingFilter::Bicubic*/) :
	_enabled(enabled),
	_ratio(ratio),
	_filter(filter)
{
}

QVariant ImageCollection::SubSampling::enabled(const int& role) const
{
	const auto enabledString = _enabled ? "true" : "false";

	switch (role)
	{
		case Qt::DisplayRole:
			return enabledString;

		case Qt::EditRole:
			return _enabled;

		case Qt::ToolTipRole:
			return QString("Subsampling enabled: %1").arg(enabledString);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::SubSampling::setEnabled(const bool& enabled)
{
	_enabled = enabled;
}

QVariant ImageCollection::SubSampling::ratio(const int& role) const
{
	const auto ratioString = QString("%1%").arg(QString::number(100.0f * _ratio, 'f', 1));

	switch (role)
	{
		case Qt::DisplayRole:
			return ratioString;

		case Qt::EditRole:
			return _ratio;

		case Qt::ToolTipRole:
			return QString("Subsampling ratio: %1").arg(ratioString);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::SubSampling::setRatio(const float& ratio)
{
	_ratio = std::clamp(ratio, 0.0f, 1.0f);
}

QVariant ImageCollection::SubSampling::filter(const int& role) const
{
	const auto filterString = imageResamplingFilterName(_filter);

	switch (role)
	{
		case Qt::DisplayRole:
			return filterString;

		case Qt::EditRole:
			return static_cast<int>(_filter);

		case Qt::ToolTipRole:
			return QString("Subsampling filter: %1").arg(filterString);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::SubSampling::setFilter(const ImageResamplingFilter& filter)
{
	_filter = filter;
}

ImageCollection::ImageCollection(TreeItem* parent, const QString& directory, const QString& imageType, const QSize& sourceSize) :
	TreeItem(parent),
	_directory(directory),
	_imageType(imageType),
	_sourceSize(sourceSize),
	_targetSize(sourceSize),
	_datasetName(),
	_toGrayscale(true),
	_type(ImageData::Type::Stack),
	_subsampling()
{
}

QVariant ImageCollection::directory(const int& role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return _directory;

		case Qt::EditRole:
			return _directory;

		case Qt::ToolTipRole:
			return QString("Directory: %1").arg(_directory);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setDirectory(const QString& directory)
{
	_directory = directory;
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

QVariant ImageCollection::targetWidth(const int& role) const
{
	const auto targetWidthString = QString::number(_targetSize.width());

	switch (role)
	{
		case Qt::DisplayRole:
			return targetWidthString;

		case Qt::EditRole:
			return _targetSize.width();

		case Qt::ToolTipRole:
			return QString("Target width: %1").arg(targetWidthString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
}

QVariant ImageCollection::targetheight(const int& role) const
{
	const auto targetHeightString = QString::number(_targetSize.height());

	switch (role)
	{
		case Qt::DisplayRole:
			return targetHeightString;

		case Qt::EditRole:
			return _targetSize.height();

		case Qt::ToolTipRole:
			return QString("Target height: %1").arg(targetHeightString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

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

QVariant ImageCollection::toGrayscale(const int& role) const
{
	const auto toGrayscaleString = _toGrayscale ? "true" : "false";

	switch (role)
	{
		case Qt::DisplayRole:
			return "";

		case Qt::EditRole:
			return _toGrayscale;

		case Qt::ToolTipRole:
			return QString("Convert images to grayscale: %1").arg(toGrayscaleString);

		case Qt::CheckStateRole:
			return _toGrayscale ? Qt::Checked : Qt::Unchecked;

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setToGrayscale(const bool& toGrayscale)
{
	_toGrayscale = toGrayscale;
}

QVariant ImageCollection::type(const int& role) const
{
	const auto typeString = ImageData::typeName(_type);

	switch (role)
	{
		case Qt::DisplayRole:
			return typeString;

		case Qt::EditRole:
			return _type;

		case Qt::ToolTipRole:
			return QString("Image collection type: %1").arg(typeString);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setType(const ImageData::Type & type)
{
	_type = type;
}

QVariant ImageCollection::noImages(const int& role) const
{
	const auto noImagesString = QString::number(childCount());

	switch (role)
	{
		case Qt::DisplayRole:
			return noImagesString;

		case Qt::EditRole:
			return childCount();

		case Qt::ToolTipRole:
			return QString("Number of images: %1").arg(noImagesString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
}

QVariant ImageCollection::noSelectedImages(const int& role) const
{
	const auto noSelectedImages = std::count_if(_children.begin(), _children.end(), [](auto& child) {
		return static_cast<Image*>(child)->shouldLoad(Qt::EditRole).toBool();
	});

	const auto noSelectedImagesString	= QString::number(noSelectedImages);

	switch (role)
	{
		case Qt::DisplayRole:
			return noSelectedImagesString;

		case Qt::EditRole:
			return noSelectedImages;

		case Qt::ToolTipRole:
			return QString("Number of selected images: %1").arg(noSelectedImagesString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
}

QVariant ImageCollection::noPoints(const int& role) const
{
	auto noPoints = 0;

	switch (_type)
	{
		case ImageData::Undefined:
			break;
		
		case ImageData::Sequence:
		{
			noPoints = noSelectedImages(Qt::EditRole).toInt();
			break;
		}

		case ImageData::Stack:
		{
			noPoints = _targetSize.width() * _targetSize.height();
			break;
		}

		case ImageData::MultiPartSequence:
			break;
		
		default:
			break;
	}

	const auto noPointsString = integerCountHumanReadable(noPoints);
	
	switch (role)
	{
		case Qt::DisplayRole:
			return noPointsString;

		case Qt::EditRole:
			return noPoints;

		case Qt::ToolTipRole:
			return QString("Number of high-dimensional data points: %1").arg(noPointsString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
}

QVariant ImageCollection::noDimensions(const int& role) const
{
	auto noDimensions = 0;

	switch (_type)
	{
		case ImageData::Undefined:
			break;

		case ImageData::Sequence:
		{
			noDimensions = _targetSize.width() * _targetSize.height(); 
			break;
		}

		case ImageData::Stack:
		{
			noDimensions = noSelectedImages(Qt::EditRole).toInt() * (_toGrayscale ? 1 : 3);
			break;
		}

		case ImageData::MultiPartSequence:
			break;

		default:
			break;
	}

	const auto noDimensionsString = integerCountHumanReadable(noDimensions);

	switch (role)
	{
		case Qt::DisplayRole:
			return noDimensionsString;

		case Qt::EditRole:
			return noDimensions;

		case Qt::ToolTipRole:
			return QString("Number of high-dimensional data dimensions: %1").arg(noDimensionsString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
}

QVariant ImageCollection::memory(const int& role) const
{
	const auto noPoints		= this->noPoints(Qt::EditRole).toInt();
	const auto noDimensions	= this->noDimensions(Qt::EditRole).toInt();
	const auto noElements	= noPoints * noDimensions;
	const auto noBytes		= noElements * sizeof(float);

	const auto memoryString = noBytesHumanReadable(noBytes);

	switch (role)
	{
		case Qt::DisplayRole:
			return memoryString;

		case Qt::EditRole:
			return noBytes;

		case Qt::ToolTipRole:
			return QString("Estimated memory consumption of the high-dimensional dataset: %1").arg(memoryString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
}

ImageCollection::SubSampling& ImageCollection::subsampling()
{
	return _subsampling;
}

void ImageCollection::addImage(const QString& filePath, const std::int32_t& pageIndex /*= -1*/)
{
	appendChild(new Image(this, filePath, pageIndex));
}

void ImageCollection::computeDatasetName()
{
	QString rootDir = "";

	QSet<QString> rootDirs;

	for (const auto& child : _children) {
		const auto path = QFileInfo(static_cast<Image*>(child)->filePath(Qt::EditRole).toString()).absoluteDir().path();

		if (rootDir == "")
			rootDir = path;

		if (path.size() < rootDir) {
			rootDir = path;

			rootDirs.insert(path);
		}
	}

	//qDebug() << rootDirs;

	if (rootDirs.count() > 1) {
		QDir childDir(rootDir);

		childDir.cdUp();

		rootDir = childDir.dirName();

		setDirectory(childDir.absolutePath());
	}
	
	setDatasetName(QString("%1").arg(QDir(rootDir).dirName()));
}

void ImageCollection::load(ImageLoaderPlugin* imageLoaderPlugin)
{
	try
	{
		const auto typeName = ImageData::typeName(_type);

		qDebug() << QString("Loading %1: %2").arg(typeName, _datasetName);

		std::vector<float> data;

		const auto noPoints = this->noPoints(Qt::EditRole).toInt();
		const auto noDimensions = this->noDimensions(Qt::EditRole).toInt();

		data.resize(noPoints * noDimensions);

		auto imageFilePaths = QStringList();
		auto dimensionNames = QStringList();

		auto imageIndex = 0;

		for (auto& childItem : _children) {
			auto image = static_cast<ImageCollection::Image*>(childItem);

			if (!image->shouldLoad(Qt::EditRole).toBool())
				continue;

			image->load(imageLoaderPlugin, data, imageIndex);

			imageIndex++;

			imageFilePaths << image->filePath(Qt::EditRole).toString();
			dimensionNames << image->dimensionName(Qt::EditRole).toString();
		}

		const auto datasetName = imageLoaderPlugin->_core->addData("Points", _datasetName);

		auto& points = dynamic_cast<Points&>(imageLoaderPlugin->_core->requestData(datasetName));

		points.setData(data.data(), static_cast<std::uint32_t>(noPoints), noDimensions);
		points.setDimensionNames(dimensionNames.toVector().toStdVector());

		points.setProperty("Type", typeName);
		points.setProperty("NoImages", noSelectedImages(Qt::EditRole).toInt());
		points.setProperty("ImageSize", targetSize(Qt::EditRole).toSize());
		points.setProperty("ImageFilePaths", imageFilePaths);
		points.setProperty("DimensionNames", dimensionNames);
	}
	catch (const std::runtime_error& e)
	{
		QMessageBox::critical(nullptr, QString("Unable to load %1").arg(_datasetName), e.what());
	}
	catch (std::exception e)
	{
		QMessageBox::critical(nullptr, QString("Unable to load %1").arg(_datasetName), e.what());
	}
}