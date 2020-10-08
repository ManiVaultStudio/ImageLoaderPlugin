#include "ImageCollectionsModel.h"
#include "ImageLoaderPlugin.h"

#include "Common.h"

#include "Application.h"

#include <QDebug>
#include <QDirIterator>
#include <QPainter>
#include <QIcon>

ImageCollectionsModel::ImageCollectionsModel(ImageLoaderPlugin* imageLoaderPlugin) :
	QAbstractItemModel(),
	_imageLoaderPlugin(imageLoaderPlugin),
	_root(new TreeItem(nullptr)),
	_selectionModel(this),
	_persistData(true)
{
}

ImageCollectionsModel::~ImageCollectionsModel()
{
	delete _root;
}

int ImageCollectionsModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		return _root->childCount();
	else {
		return static_cast<TreeItem*>(parent.internalPointer())->childCount();
	}
}

int ImageCollectionsModel::columnCount(const QModelIndex& parent) const
{
	return ult(ImageCollection::Image::Column::End) + 1;
}

QVariant ImageCollectionsModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
		return QVariant();

	if (index.parent() == QModelIndex()) {
		auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

		switch (static_cast<ImageCollection::Column>(index.column())) {
			case ImageCollection::Column::DatasetName:
				return imageCollection->getDatasetName(role);

			case ImageCollection::Column::ImageType:
				return imageCollection->getImageType(role);

			case ImageCollection::Column::ImageFormat:
				return imageCollection->getImageFormat(role);

			case ImageCollection::Column::ToGrayscale:
				return imageCollection->getToGrayscale(role);

			case ImageCollection::Column::NoImages:
				return imageCollection->getNoImages(role);

			case ImageCollection::Column::NoSelectedImages:
				return imageCollection->getNoSelectedImages(role);

			case ImageCollection::Column::SourceSize:
				return imageCollection->getSourceSize(role);

			case ImageCollection::Column::TargetSize:
				return imageCollection->getTargetSize(role);

			case ImageCollection::Column::TargetWidth:
				return imageCollection->getTargetWidth(role);

			case ImageCollection::Column::TargetHeight:
				return imageCollection->getTargetheight(role);

			case ImageCollection::Column::NoPoints:
				return imageCollection->getNoPoints(role);

			case ImageCollection::Column::NoDimensions:
				return imageCollection->getNoDimensions(role);

			case ImageCollection::Column::Memory:
				return imageCollection->getMemoryConsumption(role);

			case ImageCollection::Column::Directory:
				return imageCollection->getDirectory(role);

			case ImageCollection::Column::Type:
				return imageCollection->getType(role);

			case ImageCollection::Column::SubsamplingEnabled:
				return imageCollection->getSubsampling().getEnabled(role);

			case ImageCollection::Column::SubsamplingRatio:
				return imageCollection->getSubsampling().getRatio(role);

			case ImageCollection::Column::SubsamplingFilter:
				return imageCollection->getSubsampling().getFilter(role);
		}
	}
	else {
		auto image = static_cast<ImageCollection::Image*>((void*)index.internalPointer());

		switch (static_cast<ImageCollection::Image::Column>(index.column())) {
			case ImageCollection::Image::Column::ShouldLoad:
				return image->getShouldLoad(role);

			case ImageCollection::Image::Column::FileName:
				return image->getFileName(role);

			case ImageCollection::Image::Column::DimensionName:
				return image->getDimensionName(role);

			case ImageCollection::Image::Column::FilePath:
				return image->getFilePath(role);
		}
	}

	return QVariant();
}

bool ImageCollectionsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	QModelIndexList affectedIndices;

	affectedIndices << index;

	const auto settingsPrefix = getSettingsPrefix(index);

	if (index.parent() == QModelIndex()) {
		auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

		const auto column = static_cast<ImageCollection::Column>(index.column());

		auto updateTargetSize = [&index, &imageCollection, &affectedIndices]() {
			const auto subsamplingEnabled	= imageCollection->getSubsampling().getEnabled(Qt::EditRole).toBool();
			const auto subsamplingRatio		= imageCollection->getSubsampling().getRatio(Qt::EditRole).toFloat();
			const auto sourceSize			= imageCollection->getSourceSize(Qt::EditRole).toSize();

			auto targetSize = sourceSize;

			if (subsamplingEnabled) {
				targetSize.setWidth(std::max(static_cast<std::int32_t>(subsamplingRatio * sourceSize.width()), 1));
				targetSize.setHeight(std::max(static_cast<std::int32_t>(subsamplingRatio * sourceSize.height()), 1));
			}
			
			imageCollection->setTargetSize(targetSize);

			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetSize));
			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetWidth));
			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetHeight));
			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoPoints));
			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoDimensions));
			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::Memory));
		};

		switch (role)
		{
			case Qt::EditRole:
			{
				switch (column) {
					case ImageCollection::Column::DatasetName:
					{
						imageCollection->setDatasetName(value.toString());

						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/DatasetName", value.toString());

						break;
					}

					case ImageCollection::Column::Type:
					{
						imageCollection->setType(static_cast<ImageData::Type>(value.toInt()));

						affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoPoints));
						affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoDimensions));

						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/Type", value.toInt());

						break;
					}

					case ImageCollection::Column::SubsamplingEnabled:
					{
						imageCollection->getSubsampling().setEnabled(value.toBool());

						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/Subsampling/Enabled", value.toBool());

						updateTargetSize();
						break;
					}

					case ImageCollection::Column::SubsamplingRatio:
					{
						imageCollection->getSubsampling().setRatio(value.toFloat());
						updateTargetSize();

						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/Subsampling/Ratio", value.toFloat());

						break;
					}

					case ImageCollection::Column::SubsamplingFilter:
					{
						imageCollection->getSubsampling().setFilter(static_cast<ImageCollection::SubSampling::ImageResamplingFilter>(value.toInt()));

						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/Subsampling/Filter", value.toInt());

						break;
					}

					default:
						break;
				}

				break;
			}

			case Qt::CheckStateRole:
			{
				switch (column) {
					case ImageCollection::Column::ToGrayscale:
					{
						imageCollection->setToGrayscale(value.toBool());
						updateTargetSize();

						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/ToGrayscale", value.toBool());

						break;
					}

					default:
						break;
				}

				break;
			}

			default:
				break;
		}
	}
	else {
		auto image = static_cast<ImageCollection::Image*>((void*)index.internalPointer());

		const auto column = static_cast<ImageCollection::Image::Column>(index.column());

		switch (role)
		{
			case Qt::EditRole:
			{
				switch (column) {
					case ImageCollection::Image::Column::ShouldLoad:
					{
						image->setShouldLoad(value.toBool());

						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages));
						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoPoints));
						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoDimensions));
						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::Memory));
						
						/*
						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/Images/" + image->getDimensionName(Qt::EditRole).toString(), value.toBool());
						*/

						break;
					}

					case ImageCollection::Image::Column::FileName:
						break;

					case ImageCollection::Image::Column::DimensionName:
					{
						image->setDimensionName(value.toString());
						break;
					}

					case ImageCollection::Image::Column::FilePath:
						break;

					default:
						break;
				}

				break;
			}

			case Qt::CheckStateRole:
			{
				switch (column) {
					case ImageCollection::Image::Column::ShouldLoad:
					{
						image->setShouldLoad(value.toBool());

						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages));
						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoPoints));
						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoDimensions));
						affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::Memory));

						if (_persistData)
							_imageLoaderPlugin->setSetting(settingsPrefix + "/Images/" + image->getDimensionName(Qt::EditRole).toString(), value.toBool());

						break;
					}

					case ImageCollection::Image::Column::FileName:
					case ImageCollection::Image::Column::DimensionName:
					case ImageCollection::Image::Column::FilePath:
						break;

					default:
						break;
				}

				break;
			}

			default:
				break;
		}
	}

	for (auto affectedIndex : affectedIndices)
		emit dataChanged(affectedIndex, affectedIndex);

	return true;
}

QVariant ImageCollectionsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	auto tooltip = [](const QString& title, const QString& description) {
		return QString("<html><head/><body><p><span style='font-weight: 600;'>%1</span><br/>%2</p></body></html>").arg(title, description);
	};

	if (orientation == Qt::Horizontal) {
		switch (role)
		{
			case Qt::DecorationRole:
			{
				switch (static_cast<ImageCollection::Column>(section)) {
					case ImageCollection::Column::DatasetName:
						return hdps::Application::getIconFont("FontAwesome").getIcon("pen", QSize(13, 13), Qt::darkGray);

					default:
						break;
				}

				switch (static_cast<ImageCollection::Image::Column>(section)) {
					case ImageCollection::Image::Column::DimensionName:
						return hdps::Application::getIconFont("FontAwesome").getIcon("pen", QSize(13, 13), Qt::darkGray);

					default:
						break;
				}
				
				return QVariant();
			}

			case Qt::DisplayRole:
			{
				switch (static_cast<ImageCollection::Column>(section)) {
					case ImageCollection::Column::DatasetName:
						return "Dataset name";

					case ImageCollection::Column::ImageType:
						return "Image type";

					case ImageCollection::Column::ImageFormat:
						return "Image format";

					case ImageCollection::Column::ToGrayscale:
						return "Grayscale";

					case ImageCollection::Column::NoImages:
						return "#Images";

					case ImageCollection::Column::NoSelectedImages:
						return "#Selected images";

					case ImageCollection::Column::SourceSize:
						return "Source size";

					case ImageCollection::Column::TargetSize:
						return "Target size";

					case ImageCollection::Column::TargetWidth:
						return "Width";

					case ImageCollection::Column::TargetHeight:
						return "Height";

					case ImageCollection::Column::NoPoints:
						return "#Points";

					case ImageCollection::Column::NoDimensions:
						return "#Dimensions";

					case ImageCollection::Column::Memory:
						return "Memory";

					case ImageCollection::Column::Directory:
						return "Directory";

					case ImageCollection::Column::Type:
						return "Load as";

					case ImageCollection::Column::SubsamplingEnabled:
						return "Subsampling enabled";

					case ImageCollection::Column::SubsamplingRatio:
						return "Subsampling ratio";

					case ImageCollection::Column::SubsamplingFilter:
						return "Subsampling filter";

					default:
						break;
				}

				switch (static_cast<ImageCollection::Image::Column>(section)) {
					case ImageCollection::Image::Column::ShouldLoad:
						return "";

					case ImageCollection::Image::Column::FileName:
						return "Filename";

					case ImageCollection::Image::Column::DimensionName:
						return "Dimension name";

					case ImageCollection::Image::Column::FilePath:
						return "File path";

					default:
						break;
				}

				break;
			}

			case Qt::ToolTipRole:
			{
				switch (static_cast<ImageCollection::Column>(section)) {
					case ImageCollection::Column::DatasetName:
						return tooltip("Dataset name", "The name of the high-dimensional dataset, click to edit");

					case ImageCollection::Column::ImageType:
						return tooltip("Type", "The type of images in the scanned image collection");

					case ImageCollection::Column::NoImages:
						return tooltip("#Images", "The number of images in the collection");

					case ImageCollection::Column::NoSelectedImages:
						return tooltip("#Selected images", "The number of selected images in the collection");

					case ImageCollection::Column::SourceSize:
						return tooltip("Source size", "The size of the images on disk");

					case ImageCollection::Column::TargetSize:
						return tooltip("Target size", "The size of the images when loaded as high-dimensional data");

					case ImageCollection::Column::TargetWidth:
						return tooltip("Width", "The width of the images when loaded as high-dimensional data");

					case ImageCollection::Column::TargetHeight:
						return tooltip("Height", "The height of the images when loaded as high-dimensional data");

					case ImageCollection::Column::NoPoints:
						return tooltip("Number of points", "The number of points in the high-dimensional data");

					case ImageCollection::Column::NoDimensions:
						return tooltip("Number of dimensions", "The number of dimensions in the high-dimensional data");

					case ImageCollection::Column::Memory:
						return tooltip("Memory", "Memory consumption of the high-dimensional data");

					case ImageCollection::Column::Directory:
						return tooltip("Directory", "The top-level directory where the images were found");

					case ImageCollection::Column::Type:
						return tooltip("Load as", "How to interpret the images as high-dimensional data");

					case ImageCollection::Column::SubsamplingEnabled:
						return tooltip("Subsampling enabled", "Whether images are sub-sampled when loaded as high-dimensional data");

					case ImageCollection::Column::SubsamplingRatio:
						return tooltip("Subsampling ratio", "The amount of subsampling");

					case ImageCollection::Column::SubsamplingFilter:
						return tooltip("Subsampling filter", "The subsampling filter to use");

					case ImageCollection::Column::ToGrayscale:
						return tooltip("Convert to grayscale", "Whether all image channels are combined in to one (grayscale)");

					default:
						break;
				}

				switch (static_cast<ImageCollection::Image::Column>(section)) {
					case ImageCollection::Image::Column::ShouldLoad:
						return tooltip("Load", "Whether to load the image or not");

					case ImageCollection::Image::Column::FileName:
						return tooltip("Filename", "Name of the image file");

					case ImageCollection::Image::Column::DimensionName:
						return tooltip("Dimension name", "Name of the dimension in the high-dimensional dataset, click to edit");

					case ImageCollection::Image::Column::FilePath:
						return tooltip("File path", "The absolute file path of the image");

					default:
						break;
				}

				break;
			}
		}
	}

	return QVariant();
}

Qt::ItemFlags ImageCollectionsModel::flags(const QModelIndex& index) const
{
	auto flags = static_cast<Qt::ItemFlags>(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

	//const auto type = static_cast<Type>(_type);

	if (index.parent() == QModelIndex()) {
		switch (static_cast<ImageCollection::Column>(index.column())) {
			case ImageCollection::Column::DatasetName:
			{
				flags |= Qt::ItemIsEditable;
				break;
			}

			case ImageCollection::Column::ToGrayscale:
			{
				flags |= Qt::ItemIsUserCheckable;

				if (index != QModelIndex())
					flags |= Qt::ItemIsEditable;

				break;
			}

			case ImageCollection::Column::Directory:
			case ImageCollection::Column::SourceSize:
			case ImageCollection::Column::TargetSize:
			case ImageCollection::Column::TargetWidth:
			case ImageCollection::Column::TargetHeight:
				break;

			default:
				break;
		}
	}
	else {
		switch (static_cast<ImageCollection::Image::Column>(index.column())) {
			case ImageCollection::Image::Column::ShouldLoad:
			{
				flags |= Qt::ItemIsUserCheckable;

				if (index != QModelIndex())
					flags |= Qt::ItemIsEditable;

				break;
			}

			case ImageCollection::Image::Column::FileName:
				break;

			case ImageCollection::Image::Column::DimensionName:
			{
				flags |= Qt::ItemIsEditable;
				break;
			}

			case ImageCollection::Image::Column::FilePath:
				break;

			default:
				break;
		}
	}

	return flags;
}

QModelIndex ImageCollectionsModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = _root;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex ImageCollectionsModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->getParentItem();

	if (parentItem == _root)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

void ImageCollectionsModel::clear()
{
	beginResetModel();
	{
		_root->removeAllChildren();
	}
	endResetModel();
}

void ImageCollectionsModel::insert(int row, const std::vector<ImageCollection*>& imageCollections)
{
	if (imageCollections.empty())
		return;

	beginInsertRows(QModelIndex(), _root->childCount(), _root->childCount() + imageCollections.size() - 1);
	{
		for (auto& imageCollection : imageCollections)
			_root->appendChild(imageCollection);
	}
	endInsertRows();
	
	for (int rowIndex = 0; rowIndex < rowCount(QModelIndex()); rowIndex++) {
		const auto imageCollectionIndex		= index(rowIndex, 0);
		const auto datasetName				= data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::DatasetName)), Qt::EditRole).toString();
		const auto noImages					= data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::NoImages)), Qt::EditRole).toInt();
		const auto settingsPrefix			= getSettingsPrefix(imageCollectionIndex);

		setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::DatasetName)), _imageLoaderPlugin->getSetting(settingsPrefix + "/DatasetName", datasetName).toString());
		setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::ToGrayscale)), _imageLoaderPlugin->getSetting(settingsPrefix + "/ToGrayscale", true).toBool(), Qt::CheckStateRole);
		setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::Type)), _imageLoaderPlugin->getSetting(settingsPrefix + "/Type", ImageData::Type::Stack).toInt());
		setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingEnabled)), _imageLoaderPlugin->getSetting(settingsPrefix + "/Subsampling/Enabled", false).toBool());
		setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), _imageLoaderPlugin->getSetting(settingsPrefix + "/Subsampling/Ratio", 0.5f).toFloat());
		setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingFilter)), _imageLoaderPlugin->getSetting(settingsPrefix + "/Subsampling/Filter", 0).toInt());

		/*
		blockSignals(true);
		{
			for (int imageIndex = 0; imageIndex < noImages; imageIndex++) {
				const auto dimensionName	= index(imageIndex, ult(ImageCollection::Image::Column::DimensionName), imageCollectionIndex).data(Qt::EditRole).toString();
				const auto shouldLoad		= _imageLoaderPlugin->getSetting(settingsPrefix + "/Images/" + dimensionName, true).toBool();

				setData(imageCollectionIndex.child(imageIndex, ult(ImageCollection::Image::Column::ShouldLoad)), _imageLoaderPlugin->getSetting(settingsPrefix + "/Images/" + dimensionName, true).toBool());
			}
		}
		blockSignals(false);
		*/
	}
}

void ImageCollectionsModel::guessDimensionNames(const QModelIndex& index)
{
	if (index.parent() != QModelIndex())
		return;

	auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

	imageCollection->guessDimensionNames();
}

bool ImageCollectionsModel::loadImageCollection(ImageLoaderPlugin* imageLoaderPlugin, const QModelIndex& index)
{
	if (index.parent() != QModelIndex())
		return false;

	auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

	return imageCollection->load(imageLoaderPlugin);
}

QString ImageCollectionsModel::getSettingsPrefix(const QModelIndex& index) const
{
	const auto imageCollectionIndex		= index.parent().isValid() ? index.parent() : index;
	const auto directory				= data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::Directory)), Qt::EditRole).toString();
	const auto imageType				= data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::ImageType)), Qt::EditRole).toString();
	const auto datasetName				= data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::DatasetName)), Qt::EditRole).toString();
	
	return QString("Cache/" + QDir::fromNativeSeparators(directory) + "/" + imageType);
}

bool ImageCollectionsModel::getPersistData() const
{
	return _persistData;
}

void ImageCollectionsModel::setPersistData(const bool& persistData)
{
	_persistData = persistData;
}

void ImageCollectionsModel::selectAll(const QModelIndex& parent)
{
	if (parent.parent() != QModelIndex())
		return;

	for (std::int32_t row = 0; row < rowCount(parent); row++) {
		setData(index(row, ult(ImageCollection::Image::Column::ShouldLoad), parent), true);
	}
}

void ImageCollectionsModel::selectNone(const QModelIndex& parent)
{
	if (parent.parent() != QModelIndex())
		return;

	for (std::int32_t row = 0; row < rowCount(parent); row++) {
		setData(index(row, ult(ImageCollection::Image::Column::ShouldLoad), parent), false);
	}
}

void ImageCollectionsModel::invertSelection(const QModelIndex& parent)
{
	if (parent.parent() != QModelIndex())
		return;

	auto imageCollection = static_cast<ImageCollection*>((void*)parent.internalPointer());

	for (std::int32_t row = 0; row < rowCount(parent); row++) {
		const auto imageIndex = index(row, ult(ImageCollection::Image::Column::ShouldLoad), parent);
		const auto shouldLoad = data(imageIndex, Qt::EditRole).toBool();

		setData(imageIndex, !shouldLoad);
	}
}

void ImageCollectionsModel::selectPercentage(const QModelIndex& parent, const float& selectionProbability)
{
	if (parent.parent() != QModelIndex())
		return;

	auto imageCollection = static_cast<ImageCollection*>((void*)parent.internalPointer());

	const auto probability		= std::clamp(selectionProbability, 0.0f, 1.0f);
	const auto shouldLoadColumn	= ult(ImageCollection::Image::Column::ShouldLoad);

	for (std::int32_t row = 0; row < rowCount(parent); row++) {
		const auto random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		setData(index(row, shouldLoadColumn, parent), random < selectionProbability);
	}
}