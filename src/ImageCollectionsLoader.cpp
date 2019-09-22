#include "ImageCollectionsLoader.h"
#include "ImageUtilities.h"
#include "ImageScanner.h"

#include <QDebug>
#include <QFileInfo>

ImageCollectionsLoader::ImageCollectionsLoader(const ImageCollectionType& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(imageCollectionTypeName(type))),
	_type(type),
	_datasetName(),
	_subsampleImageSettings(&_settings)
{
}

ImageCollectionType ImageCollectionsLoader::type() const
{
	return _type;
}

SubsampleImageSettings & ImageCollectionsLoader::subsampleImageSettings()
{
	return _subsampleImageSettings;
}

QVariant ImageCollectionsLoader::setting(const QString& name, const QVariant& defaultValue) const
{
	return _settings.value(name, defaultValue).toString();
}

void ImageCollectionsLoader::setSetting(const QString& name, const QVariant& value)
{
	_settings.setValue(name, value);
}

QString ImageCollectionsLoader::datasetName() const
{
	return _datasetName;
}

void ImageCollectionsLoader::setDatasetName(const QString& datasetName)
{
	if (datasetName == _datasetName)
		return;

	_datasetName = datasetName;

	emit datasetNameChanged(_datasetName);
}

void ImageCollectionsLoader::load(const ImageCollections& scannedImageCollections)
{
	emit beginLoad();

	ImageCollections imageCollections = scannedImageCollections;

	auto imagePointDataSet = ImagePointDataSet(_type);

	imagePointDataSet.setName(_datasetName);

	switch (_type)
	{
		case ImageCollectionType::Sequence:
		{
			ImageCollection sequence = imageCollections.map().first();

			if (_subsampleImageSettings.enabled())
				imagePointDataSet.setImageSize("0", sequence.imageSize() * (_subsampleImageSettings.ratio() / 100.f));
			else
				imagePointDataSet.setImageSize("0", sequence.imageSize());

			const auto imageFilePaths		= sequence.imageFilePaths();
			const auto noImages				= sequence.noImages();
			const auto imageSize			= imagePointDataSet.imageSize("0");
			const auto noPointsPerDimension = imagePointDataSet.noPointsPerDimension();
			const auto noDimensions			= noPointsPerDimension;
			const auto total				= noImages;

			imagePointDataSet.setImageFilePaths(imageFilePaths);
			imagePointDataSet.setNoDimensions(noDimensions);

			auto& pointsData = imagePointDataSet.create();

			qDebug() << imagePointDataSet;

			foreach(const QString& imageFilePath, imageFilePaths) {
				const auto imageIndex = imageFilePaths.indexOf(imageFilePath);

				const auto pointIndexMapper = [noPointsPerDimension, imageIndex, imageSize](int x, int y) {
					const auto localPixelIndex = y * imageSize.width() + x;
					return imageIndex * noPointsPerDimension + localPixelIndex;
				};

				loadImage(imageFilePath, imageSize, pointIndexMapper, pointsData);

				const auto done			= imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(total));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
			}

			emit message(QString("%1 image(s) loaded").arg(total));

			break;
		}

		case ImageCollectionType::Stack:
		{
			ImageCollection stack = imageCollections.map().first();

			if (_subsampleImageSettings.enabled())
				imagePointDataSet.setImageSize("0", stack.imageSize() * (_subsampleImageSettings.ratio() / 100.f));
			else
				imagePointDataSet.setImageSize("0", stack.imageSize());

			imagePointDataSet.setImageFilePaths(stack.imageFilePaths());
			imagePointDataSet.setDimensionNames(imagePointDataSet.imageFilePaths());
			imagePointDataSet.setNoDimensions(imagePointDataSet.noImages());

			const auto noImages		= imagePointDataSet.noImages();
			const auto imageSize	= imagePointDataSet.imageSize("0");
			const auto noDimensions	= noImages;
			
			//qDebug() << imagePointDataSet.imageSizes();
			//qDebug() << imagePointDataSet.noImages();
			//qDebug() << imageSize;

			auto& pointsData = imagePointDataSet.create();

			foreach(const QString& imageFilePath, imagePointDataSet.imageFilePaths()) {
				const auto imageIndex = imagePointDataSet.imageFilePaths().indexOf(imageFilePath);

				const auto pointIndexMapper = [noImages, imageIndex, imageSize](int x, int y) {
					const auto localPixelIndex = y * imageSize.width() + x;
					return localPixelIndex * noImages + imageIndex;
				};

				loadImage(imageFilePath, imageSize, pointIndexMapper, pointsData);

				const auto done			= imagePointDataSet.imageFilePaths().indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(noImages));

				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages), QString::number(percentage, 'f', 1)));
			}

			emit message(QString("%1 image(s) loaded").arg(noImages));
			
			break;
		}

		case ImageCollectionType::MultiPartSequence:
		{
			auto imageFilePaths = QStringList();

			imagePointDataSet.setNoDimensions(imageCollections.map().first().noDimensions());

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath = imageCollection.imageFilePaths().first();

				imageFilePaths << imageFilePath;

				auto imageSize = imageCollection.imageSize();

				if (_subsampleImageSettings.enabled())
					imageSize = imageCollection.imageSize() * (_subsampleImageSettings.ratio() / 100.f);

				imagePointDataSet.setImageSize(imageFilePath, imageSize);
			}

			const auto noImages = imageFilePaths.size();

			imagePointDataSet.setImageFilePaths(imageFilePaths);
			
			auto dimensionIds = std::vector<int>();

			dimensionIds.resize(imagePointDataSet.noDimensions());
			std::iota(std::begin(dimensionIds), std::end(dimensionIds), 0);

			auto dimensionNames = QStringList();

			for (int dimensionId : dimensionIds)
			{
				dimensionNames << QString("dim_%1").arg(dimensionId);
			}

			imagePointDataSet.setDimensionNames(dimensionNames);
			
			auto& pointsData = imagePointDataSet.pointsData();

			pointsData.clear();
			pointsData.resize(imagePointDataSet.noPointsTotal());
			
			emit message(QString("Loading %1 images").arg(QString::number(noImages)));
			
			auto noPointsPerDimension = 0;

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath	= imageCollection.imageFilePaths().first();
				const auto imageSize		= imagePointDataSet.imageSize(imageFilePath);

				noPointsPerDimension += imageSize.width() * imageSize.height();
			}

			qDebug() << "LOADER::noPointsPerDimension" << noPointsPerDimension;
			
			int imageOffset	= 0;
			int done			= 0;

			foreach(const ImageCollection& imageCollection, imageCollections.map()) {
				const auto imageFilePath	= imageCollection.imageFilePaths().first();
				const auto imageIndex		= imageFilePaths.indexOf(imageFilePath);
				const auto imageSize		= imagePointDataSet.imageSize(imageFilePath);
				
				auto* multiBitmap = freeImageOpenMultiBitmap(imageFilePath);

				if (multiBitmap != nullptr) {
					const auto noPages = FreeImage_GetPageCount(multiBitmap);

					qDebug() << "LOADER::imageOffset" << imageOffset;

					for (int pageIndex = 0; pageIndex < noPages; pageIndex++) {
						const auto pointIndexMapper = [imageOffset, noPointsPerDimension, pageIndex, imageSize](int x, int y) {
							const auto localPixelIndex = y * imageSize.width() + x;
							return (pageIndex * noPointsPerDimension) + imageOffset + localPixelIndex;
						};

						auto* pageBitmap = FreeImage_LockPage(multiBitmap, pageIndex);

						if (pageBitmap != nullptr) {
							//qDebug() << pageIndex;

							loadBitmap(pageBitmap, imageSize, pointIndexMapper, pointsData);

							FreeImage_UnlockPage(multiBitmap, pageBitmap, false);
						}
					}

					imageOffset += imageSize.width() * imageSize.height();

					done++;

					FreeImage_CloseMultiBitmap(multiBitmap);

					const auto percentage = 100.0f * (done / static_cast<float>(noImages));

					emit message(QString("Loaded %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(noImages), QString::number(percentage, 'f', 1)));
				}
			}

			break;
		}

		default:
			break;
	}

	qDebug() << imagePointDataSet;

	emit endLoad(imagePointDataSet);
}

template<typename PointIndexMapper>
void ImageCollectionsLoader::loadBitmap(FIBITMAP* bitmap, const QSize& imageSize, const PointIndexMapper& pointIndexMapper, FloatVector& pointsData)
{
	assert(bitmap != nullptr);

	const auto image_type = FreeImage_GetImageType(bitmap);

	auto* greyscaleBitmap = FreeImage_ConvertToGreyscale(bitmap);

	if (greyscaleBitmap) {

		const auto width	= FreeImage_GetWidth(greyscaleBitmap);
		const auto height	= FreeImage_GetHeight(greyscaleBitmap);
		const auto rescale	= QSize(width, height) != imageSize;

		auto* rescaledBitmap = rescale ? FreeImage_Rescale(greyscaleBitmap, imageSize.width(), imageSize.height(), static_cast<FREE_IMAGE_FILTER>(_subsampleImageSettings.filter())) : greyscaleBitmap;

		if (rescaledBitmap) {
			const auto scaledWidth = imageSize.width();

			for (int y = 0; y < imageSize.height(); y++) {
				switch (image_type)
				{
					case FIT_BITMAP:
					{
						const BYTE *const bits = FreeImage_GetScanLine(rescaledBitmap, y);

						for (int x = 0; x < scaledWidth; x++) {
							pointsData[pointIndexMapper(x, y)] = static_cast<float>(bits[x]);
						}
						break;
					}

					case FIT_UINT16:
					{
						unsigned short* bits = (unsigned short *)FreeImage_GetScanLine(rescaledBitmap, y);

						for (int x = 0; x < scaledWidth; x++) {
							pointsData[pointIndexMapper(x, y)] = static_cast<float>(bits[x]);
						}
						break;
					}
				}
			}

			FreeImage_Unload(rescaledBitmap);
		}

		if (rescale)
			FreeImage_Unload(greyscaleBitmap);
	}

/*
		case FIT_UINT16:
		{
			//qDebug() << "FIT_UINT16";
			auto* greyscaleBitmap = FreeImage_ConvertToGreyscale(bitmap);

			const auto width = FreeImage_GetWidth(greyscaleBitmap);
			const auto height = FreeImage_GetHeight(greyscaleBitmap);
			const auto rescale = QSize(width, height) != imageSize;

			auto* rescaledBitmap = rescale ? FreeImage_Rescale(greyscaleBitmap, imageSize.width(), imageSize.height(), static_cast<FREE_IMAGE_FILTER>(_subsampleImageSettings.filter())) : greyscaleBitmap;

			if (rescaledBitmap) {
				for (int y = 0; y < FreeImage_GetHeight(rescaledBitmap); y++) {
					unsigned short *bits = (unsigned short *)FreeImage_GetScanLine(rescaledBitmap, y);
					for (int x = 0; x < FreeImage_GetWidth(rescaledBitmap); x++) {
						//if (pointIndexMapper(x, y) > pointsData.size())
						//	qDebug() << "Problem!";
						pointsData[pointIndexMapper(x, y)] = static_cast<float>(bits[x]);
						//if (bits[x] > 100)
						//	qDebug() << "FIT_UINT16" << bits[x];
					}
				}
			}
			//FreeImage_Unload(greyscaleBitmap);

			break;
		}

		default:
			break;
	}
	*/
	
}

template<typename PointIndexMapper>
void ImageCollectionsLoader::loadImage(const QString& imageFilePath, const QSize& imageSize, const PointIndexMapper& pointIndexMapper, FloatVector& pointsData)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);

	auto* bitmap = freeImageLoad(imageFilePath);
	
	if (bitmap) {
		loadBitmap(bitmap, imageSize, pointIndexMapper, pointsData);

		FreeImage_Unload(bitmap);
	}
}