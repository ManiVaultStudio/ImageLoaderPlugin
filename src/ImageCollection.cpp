#include "ImageCollection.h"
#include "ImageLoaderPlugin.h"
#include "Common.h"

#include "PointData.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QPushButton>

#include <algorithm>
#include <stdexcept> // For runtime_error.

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
	auto* pageBitmap = FI::FreeImage_LockPage(multiBitmap, pageIndex);

	FITAG* tag;

	FI::FreeImage_GetMetadata(FREE_IMAGE_MDMODEL::FIMD_CUSTOM, pageBitmap, "DESCRIPTION", &tag);
	
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

void ImageCollection::Image::load(ImageLoaderPlugin* imageLoaderPlugin, std::vector<float>& data, const std::uint32_t& imageIndex, QStringList& dimensionNames, FI::FIMULTIBITMAP* multiBitmap /*= nullptr*/)
{
	qDebug() << QString("Loading image: %1").arg(_fileName);

	if (_pageIndex >= 0)
	{
		FI::FIBITMAP* pageBitmap = nullptr;

		const auto handleException = [&](const QString& message) {
			FI::FreeImage_Unload(pageBitmap);

			throw std::runtime_error(QString("Error loading %1: %2").arg(_fileName, message).toLatin1());
		};
		
		try
		{
			if (multiBitmap == nullptr)
				throw std::runtime_error("Multi-bitmap handle is not valid");

			pageBitmap = FI::FreeImage_LockPage(multiBitmap, _pageIndex);

			if (pageBitmap == nullptr)
				throw std::runtime_error("Unable to open multi-bitmap page");

			loadBitmap(pageBitmap, data, imageIndex, dimensionNames);
			
			FI::FreeImage_UnlockPage(multiBitmap, pageBitmap, false);
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
	else {

#ifdef _WIN32
		const void* const voidPtr	= _filePath.utf16();
		const auto wcharPtr			= static_cast<const wchar_t*>(voidPtr);
		const auto format			= FI::FreeImage_GetFileTypeU(wcharPtr);

		auto bitmap = FI::FreeImage_LoadU(format, wcharPtr);
#else
		const auto utf8		= _filePath.toUtf8();
		const auto format	= FI::FreeImage_GetFileType(utf8);

		auto bitmap = FI::FreeImage_Load(format, utf8);
#endif

		if (bitmap == nullptr)
			throw std::runtime_error("Unable to load bitmap");

		loadBitmap(bitmap, data, imageIndex, dimensionNames);

		FI::FreeImage_Unload(bitmap);
	}
}

template<class T> static void readSequence(ImageCollection* imageCollection, FI::FIBITMAP* bitmap, std::vector<float>& data, const std::uint32_t& imageIndex, const std::int32_t& noComponents = 1)
{
	if (bitmap == nullptr)
		throw std::runtime_error("Bitmap handle is NULL");

	const auto targetSize	= imageCollection->targetSize(Qt::EditRole).toSize();
	const auto targetWidth	= targetSize.width();
	const auto targetHeight	= targetSize.height();
	const auto noPixels		= targetWidth * targetHeight;
	const auto grayscale	= imageCollection->toGrayscale(Qt::EditRole).toBool();

	for (std::int32_t y = 0; y < targetHeight; y++) {
		auto scanLine = reinterpret_cast<T*>(FI::FreeImage_GetScanLine(bitmap, y));

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
static void readStack(ImageCollection* imageCollection, FI::FIBITMAP* bitmap, std::vector<float>& data, const std::uint32_t& imageIndex, const std::uint32_t& noDimensions, const std::int32_t& noComponents = 1)
{
	if (bitmap == nullptr)
		throw std::runtime_error("Bitmap handle is NULL");

	const auto targetSize	= imageCollection->targetSize(Qt::EditRole).toSize();
	const auto targetWidth	= targetSize.width();
	const auto targetHeight	= targetSize.height();
	const auto noPixels		= targetWidth * targetHeight;
	const auto grayscale	= imageCollection->toGrayscale(Qt::EditRole).toBool();

	for (std::int32_t y = 0; y < targetHeight; y++) {
		auto scanLine = reinterpret_cast<T*>(FI::FreeImage_GetScanLine(bitmap, y));

		if (scanLine == nullptr)
			throw std::runtime_error("Unable to read scan line");

		for (std::int32_t x = 0; x < targetWidth; x++) {
			const auto pixelIndex = y * targetWidth + x;

			if (noComponents == 1)
				data[pixelIndex * noDimensions + imageIndex] = static_cast<float>(scanLine[x]);

			if (noComponents == 3 || noComponents == 4) {
				if (grayscale) {
					auto sum = 0.0f;

					for (std::int32_t c = 0; c < 3; c++)
						sum += static_cast<float>(scanLine[x * noComponents + c]);

					data[pixelIndex * noDimensions + imageIndex] = sum / 3.0f;
				} else {
					for (std::int32_t c = 0; c < 3; c++)
						data[pixelIndex * noDimensions + (imageIndex + c)] = static_cast<float>(scanLine[x * noComponents + c]);
				}
			}
		}
	}
}

void ImageCollection::Image::loadBitmap(FI::FIBITMAP* bitmap, std::vector<float>& data, const std::uint32_t& imageIndex, QStringList& dimensionNames)
{
	FI::FIBITMAP* subsampledBitmap	= nullptr;
	FI::FIBITMAP* convertedBitmap	= nullptr;

	const auto handleException = [&](const QString& message) {
		if (subsampledBitmap != bitmap)
			FI::FreeImage_Unload(subsampledBitmap);

		FI::FreeImage_Unload(convertedBitmap);

		throw std::runtime_error(QString("Error loading %1: %2").arg(_fileName, message).toLatin1());
	};

	try {
		if (bitmap == nullptr)
			throw std::runtime_error("FI::FreeImage bitmap handle is NULL");

		const auto imageCollectionType	= static_cast<ImageData::Type>(imageCollection()->type(Qt::EditRole).toInt());
		const auto sourceWidth			= FI::FreeImage_GetWidth(bitmap);
		const auto sourceHeight			= FI::FreeImage_GetHeight(bitmap);
		const auto targetSize			= imageCollection()->targetSize(Qt::EditRole).toSize();
		const auto targetWidth			= targetSize.width();
		const auto targetHeight			= targetSize.height();
		const auto noDimensions			= imageCollection()->noDimensions(Qt::EditRole).toInt();
		const auto subsample			= QSize(sourceWidth, sourceHeight) != targetSize;
		const auto filter				= static_cast<FI::FREE_IMAGE_FILTER>(imageCollection()->subsampling().filter(Qt::EditRole).toInt());

		subsampledBitmap = subsample ? FI::FreeImage_Rescale(bitmap, targetWidth, targetHeight, filter) : bitmap;

		if (subsampledBitmap == nullptr)
			throw std::runtime_error("Unable to subsample bitmap");

		const auto imageType = FI::FreeImage_GetImageType(bitmap);

		auto noComponents = 0;

		switch (imageType)
		{
			case FI::FIT_UINT16:
			{
				noComponents = 1;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::uint16_t>(imageCollection(), subsampledBitmap, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::uint16_t>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions);

				break;
			}

			case FI::FIT_INT16:
			{
				noComponents = 1;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::int16_t>(imageCollection(), subsampledBitmap, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::int16_t>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions);

				break;
			}

			case FI::FIT_UINT32:
			{
				noComponents = 1;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::uint32_t>(imageCollection(), subsampledBitmap, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::uint32_t>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions);

				break;
			}

			case FI::FIT_INT32:
			{
				noComponents = 1;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::int32_t>(imageCollection(), subsampledBitmap, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::int32_t>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions);

				break;
			}

			case FI::FIT_FLOAT:
			{
				noComponents = 1;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<float>(imageCollection(), subsampledBitmap, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<float>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions);

				break;
			}
			
			case FI::FIT_DOUBLE:
			{
				noComponents = 1;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<double>(imageCollection(), subsampledBitmap, data, imageIndex);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<double>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions);

				break;
			}

			case FI::FIT_COMPLEX:
				break;

			case FI::FIT_BITMAP:
			{
				const auto bpp = FI::FreeImage_GetBPP(subsampledBitmap);

				switch (bpp)
				{
					case 8:
						noComponents = 1;
						break;
					
					case 16:
						noComponents = 2;
						break;

					case 24:
						noComponents = 3;
						break;

					case 32:
						noComponents = 4;
						break;

					default:
						break;
				}

				if (noComponents > 0) {
					if (imageCollectionType == ImageData::Type::Sequence)
						readSequence<FI::BYTE>(imageCollection(), subsampledBitmap, data, imageIndex, noComponents);

					if (imageCollectionType == ImageData::Type::Stack)
						readStack<FI::BYTE>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions, noComponents);
				}

				break;
			}

			case FI::FIT_RGB16:
				break;

			case FI::FIT_RGBF:
			{
				noComponents = 3;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<float>(imageCollection(), subsampledBitmap, data, imageIndex, 3);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<float>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions, 3);

				break;
			}

			case FI::FIT_RGBA16:
			{
				noComponents = 4;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<std::uint16_t>(imageCollection(), subsampledBitmap, data, imageIndex, noComponents);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<std::uint16_t>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions, noComponents);

				break;
			}

			case FI::FIT_RGBAF:
			{
				noComponents = 4;

				if (imageCollectionType == ImageData::Type::Sequence)
					readSequence<float>(imageCollection(), subsampledBitmap, data, imageIndex, noComponents);

				if (imageCollectionType == ImageData::Type::Stack)
					readStack<float>(imageCollection(), subsampledBitmap, data, imageIndex, noDimensions, noComponents);

				break;
			}

			default:
				break;
		}
		
		if (imageCollection()->toGrayscale(Qt::EditRole).toBool()) {
			dimensionNames << dimensionName(Qt::EditRole).toString();
		}
		else {
			switch (noComponents)
			{
				case 1:
				{
					dimensionNames << dimensionName(Qt::EditRole).toString();
					break;
				}

				case 3:
				case 4:
				{
					const auto dimensionNameBase = dimensionName(Qt::EditRole).toString();

					dimensionNames << QString("%1_red").arg(dimensionNameBase);
					dimensionNames << QString("%1_green").arg(dimensionNameBase);
					dimensionNames << QString("%1_blue").arg(dimensionNameBase);

					break;
				}

				default:
					break;
			}
		}

		FI::FreeImage_Unload(convertedBitmap);

		if (subsampledBitmap != bitmap)
			FI::FreeImage_Unload(subsampledBitmap);
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

void ImageCollection::Image::guessDimensionName()
{
	if (isFlagSet(ult(Flag::DimensionNamesGuessed)))
		return;

	try
	{
		if (_pageIndex >= 0) {
			FI::FIMULTIBITMAP* multiBitmap = nullptr;

			const auto fileNameUtf8	= _filePath.toUtf8();
			const auto format		= FI::FreeImage_GetFileType(fileNameUtf8);

			multiBitmap = FI::FreeImage_OpenMultiBitmap(FI::FIF_TIFF, fileNameUtf8, false, false, false);

			if (multiBitmap == nullptr)
				throw std::runtime_error("Unable to open multi- bitmap");

			auto pageBitmap = FI::FreeImage_LockPage(multiBitmap, _pageIndex);

			if (pageBitmap == nullptr)
				throw std::runtime_error("Unable to open multi-bitmap page");

			FI::FITAG* tagPageName = nullptr;

			FI::FreeImage_GetMetadata(FI::FIMD_EXIF_MAIN, pageBitmap, "PageName", &tagPageName);

			if (tagPageName != nullptr) {
				_dimensionName = (char*)FI::FreeImage_GetTagValue(tagPageName);
			}

			FI::FITAG* tagImageDescription = nullptr;

			FI::FreeImage_GetMetadata(FI::FIMD_EXIF_MAIN, pageBitmap, "ImageDescription", &tagImageDescription);

			if (tagImageDescription != nullptr) {
				_dimensionName = (char*)FI::FreeImage_GetTagValue(tagImageDescription);
			}

			if (_dimensionName.isEmpty())
				_dimensionName = QString("Dim %1").arg(_pageIndex);

			FI::FreeImage_UnlockPage(multiBitmap, pageBitmap, false);
			FI::FreeImage_CloseMultiBitmap(multiBitmap);
		}
		else {
			_dimensionName = _fileName;
		}
	}
	catch (const std::runtime_error& e)
	{
		throw e;
	}
	catch (std::exception e)
	{
		throw e;
	}
}

ImageCollection* ImageCollection::Image::imageCollection()
{
	return static_cast<ImageCollection*>(parentItem());
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

ImageCollection::ImageCollection(TreeItem* parent, const QString& directory, const QString& imageType, const QImage::Format& imageFormat, const QSize& sourceSize) :
	TreeItem(parent),
	_directory(directory),
	_imageFileType(imageType),
	_imageFormat(imageFormat),
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
			return _imageFileType;

		case Qt::EditRole:
			return _imageFileType;

		case Qt::ToolTipRole:
			return QString("Image type: %1").arg(_imageFileType);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setImageType(const QString& imageType)
{
	_imageFileType = imageType;
}

QVariant ImageCollection::imageFormat(const int& role) const
{
	auto formatString = "";

	switch (_imageFormat)
	{
		case QImage::Format_Invalid:
		case QImage::Format_Mono:
		case QImage::Format_MonoLSB:
		case QImage::Format_Indexed8:
			break;

		case QImage::Format_RGB32:
			formatString = "RGB32";
			break;

		case QImage::Format_ARGB32:
			formatString = "ARGB32";
			break;

		case QImage::Format_ARGB32_Premultiplied:
			formatString = "ARGB32_Premultiplied";
			break;

		case QImage::Format_RGB16:
			formatString = "RGB16";
			break;

		case QImage::Format_ARGB8565_Premultiplied:
		case QImage::Format_RGB666:
		case QImage::Format_ARGB6666_Premultiplied:
		case QImage::Format_RGB555:
		case QImage::Format_ARGB8555_Premultiplied:
			break;

		case QImage::Format_RGB888:
			formatString = "RGB888";
			break;

		case QImage::Format_RGB444:
		case QImage::Format_ARGB4444_Premultiplied:
		case QImage::Format_RGBX8888:
			break;

		case QImage::Format_RGBA8888:
			formatString = "RGBA8888";
			break;

		case QImage::Format_RGBA8888_Premultiplied:
			formatString = "RGBA8888_Premultiplied";
			break;

		case QImage::Format_BGR30:
		case QImage::Format_A2BGR30_Premultiplied:
		case QImage::Format_RGB30:
		case QImage::Format_A2RGB30_Premultiplied:
		case QImage::Format_Alpha8:
			break;

		case QImage::Format_Grayscale8:
			formatString = "Grayscale8";
			break;

		case QImage::Format_RGBX64:
			formatString = "RGBX64";
			break;

		case QImage::Format_RGBA64:
			formatString = "RGBA64";
			break;

		case QImage::Format_RGBA64_Premultiplied:
			formatString = "Grayscale8";
			break;

		case QImage::Format_Grayscale16:
			formatString = "Grayscale16";
			break;

		case QImage::Format_BGR888:
			formatString = "BGR888";
			break;

		case QImage::NImageFormats:
			break;

		default:
			break;
	}

	switch (role)
	{
		case Qt::DisplayRole:
			return formatString;

		case Qt::EditRole:
			return _imageFormat;

		case Qt::ToolTipRole:
			return QString("Image format: %1").arg(formatString);

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
}

void ImageCollection::setImageFormat(const QImage::Format& imageFormat)
{
	_imageFormat = imageFormat;
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

ImageCollection::Image* ImageCollection::image(const std::uint32_t& index)
{
	return static_cast<Image*>(_children[index]);
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

	const auto memoryString = noBytesHumanReadable(static_cast<std::int32_t>(noBytes));

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
	if (noImages(Qt::EditRole).toInt() == 1 || image(0)->pageIndex(Qt::EditRole).toInt() >= 0) {
		setDatasetName(image(0)->fileName(Qt::EditRole).toString());
		return;
	}

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

void ImageCollection::guessDimensionNames()
{
	if (_children.isEmpty())
		return;

	try
	{
		for (auto child : _children) {
			static_cast<Image*>(child)->guessDimensionName();
		}
	}
	catch (const std::runtime_error& e)
	{
		QMessageBox::critical(nullptr, QString("Unable to guess dimensions names for %1").arg(_datasetName), e.what());
	}
	catch (std::exception e)
	{
		QMessageBox::critical(nullptr, QString("Unable to guess dimensions names for %1").arg(_datasetName), e.what());
	}
}

bool ImageCollection::load(ImageLoaderPlugin* imageLoaderPlugin)
{
	try
	{
		const auto typeName = ImageData::typeName(_type);

		QProgressDialog progressDialog("Loading", "Abort loading", 0, noSelectedImages(Qt::EditRole).toInt(), nullptr);

		progressDialog.setWindowTitle(QString("Loading image %1: %2").arg(typeName.toLower(), _datasetName));
		progressDialog.setWindowModality(Qt::WindowModal);
		progressDialog.setMinimumDuration(100);
		progressDialog.setFixedWidth(600);
		progressDialog.show();

		qDebug() << QString("Loading %1: %2").arg(typeName, _datasetName);

		std::vector<float> data;

		const auto noPoints = this->noPoints(Qt::EditRole).toInt();
		const auto noDimensions = this->noDimensions(Qt::EditRole).toInt();

		data.resize(noPoints * noDimensions);

		auto imageFilePaths = QStringList();
		auto dimensionNames = QStringList();

		auto imageIndex = 0;

		const auto firstChild		= static_cast<ImageCollection::Image*>(_children.first());
		const auto multiPageTiff	= firstChild->pageIndex(Qt::EditRole).toInt() >= 0;
		
		FI::FIMULTIBITMAP* multiBitmap = nullptr;

		if (multiPageTiff) {
			const auto fileNameUtf8	= firstChild->filePath(Qt::EditRole).toString().toUtf8();
			const auto format		= FI::FreeImage_GetFileType(fileNameUtf8);

			multiBitmap = FI::FreeImage_OpenMultiBitmap(FI::FIF_TIFF, fileNameUtf8, false, false, false);

			if (multiBitmap == nullptr)
				throw std::runtime_error("Unable to open multi- bitmap");
		}

		for (auto& childItem : _children) {
			if (progressDialog.wasCanceled()) {
				throw std::runtime_error("Loading was aborted");
			}

			auto image = static_cast<ImageCollection::Image*>(childItem);

			progressDialog.setLabelText(QString("Loading %1").arg(image->dimensionName(Qt::EditRole).toString()));

			QCoreApplication::processEvents();

			if (!image->shouldLoad(Qt::EditRole).toBool())
				continue;

			image->load(imageLoaderPlugin, data, imageIndex, dimensionNames, multiBitmap);

			imageIndex++;

			progressDialog.setValue(imageIndex);

			imageFilePaths << image->filePath(Qt::EditRole).toString();
		}

		if (multiBitmap != nullptr)
			FI::FreeImage_CloseMultiBitmap(multiBitmap);

		const auto datasetName = imageLoaderPlugin->_core->addData("Points", _datasetName);

		auto& points = dynamic_cast<Points&>(imageLoaderPlugin->_core->requestData(datasetName));

		points.setData(std::move(data), noDimensions);
		points.setDimensionNames(std::vector<QString>(dimensionNames.begin(), dimensionNames.end()));

		points.setProperty("Type", "Images");
		points.setProperty("CollectionType", _type);
		points.setProperty("ImageSize", _targetSize);
		points.setProperty("NoImages", noSelectedImages(Qt::EditRole).toInt());
		points.setProperty("ImageSize", targetSize(Qt::EditRole).toSize());
		points.setProperty("ImageFilePaths", imageFilePaths);
		points.setProperty("DimensionNames", dimensionNames);

		imageLoaderPlugin->_core->notifyDataAdded(datasetName);

		return true;
	}
	catch (const std::runtime_error& e)
	{
		QMessageBox::critical(nullptr, QString("Unable to load %1").arg(_datasetName), e.what());
	}
	catch (std::exception e)
	{
		QMessageBox::critical(nullptr, QString("Unable to load %1").arg(_datasetName), e.what());
	}

	return false;
}