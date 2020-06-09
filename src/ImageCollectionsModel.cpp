#include "ImageCollectionsModel.h"

#include "Common.h"

#include <QDebug>
#include <QDirIterator>
#include <QPainter>

ImageCollectionsModel::ImageCollectionsModel() :
	QAbstractItemModel(),
	//_imageCollections(),
	_root(new TreeItem(nullptr)),
	_selectionModel(this)
{
}

ImageCollectionsModel::~ImageCollectionsModel()
{
	delete _root;
}

int ImageCollectionsModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	TreeItem* parentItem;

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
				return imageCollection->datasetName(role);

			case ImageCollection::Column::ImageType:
				return imageCollection->imageType(role);

			case ImageCollection::Column::NoImages:
				return imageCollection->noImages(role);

			case ImageCollection::Column::NoSelectedImages:
				return imageCollection->noSelectedImages(role);

			case ImageCollection::Column::ToGrayscale:
				return imageCollection->toGrayscale(role);

			case ImageCollection::Column::SourceSize:
				return imageCollection->sourceSize(role);

			case ImageCollection::Column::TargetSize:
				return imageCollection->targetSize(role);

			case ImageCollection::Column::TargetWidth:
				return imageCollection->targetWidth(role);

			case ImageCollection::Column::TargetHeight:
				return imageCollection->targetheight(role);

			case ImageCollection::Column::Directory:
				return imageCollection->directory(role);

			case ImageCollection::Column::Type:
				return imageCollection->type(role);

			case ImageCollection::Column::SubsamplingEnabled:
				return imageCollection->subsampling().enabled(role);

			case ImageCollection::Column::SubsamplingRatio:
				return imageCollection->subsampling().ratio(role);

			case ImageCollection::Column::SubsamplingFilter:
				return imageCollection->subsampling().filter(role);
		}
	}
	else {
		auto image = static_cast<ImageCollection::Image*>((void*)index.internalPointer());

		switch (static_cast<ImageCollection::Image::Column>(index.column())) {
			case ImageCollection::Image::Column::ShouldLoad:
				return image->shouldLoad(role);

			case ImageCollection::Image::Column::FileName:
				return image->fileName(role);

			case ImageCollection::Image::Column::DimensionName:
				return image->dimensionName(role);

			case ImageCollection::Image::Column::FilePath:
				return image->filePath(role);
		}
	}

	return QVariant();
}

bool ImageCollectionsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	QModelIndexList affectedIndices;

	affectedIndices << index;

	if (index.parent() == QModelIndex()) {
		auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

		const auto column = static_cast<ImageCollection::Column>(index.column());

		auto updateTargetSize = [&index, &imageCollection, &affectedIndices]() {
			const auto subsamplingEnabled	= imageCollection->subsampling().enabled(Qt::EditRole).toBool();
			const auto subsamplingRatio		= imageCollection->subsampling().ratio(Qt::EditRole).toFloat();
			const auto sourceSize			= imageCollection->sourceSize(Qt::EditRole).toSize();

			imageCollection->setTargetSize(subsamplingEnabled ? subsamplingRatio * sourceSize : sourceSize);

			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetSize));
			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetWidth));
			affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetHeight));
		};

		switch (role)
		{
			case Qt::EditRole:
			{
				switch (column) {
					case ImageCollection::Column::DatasetName:
					{
						imageCollection->setDatasetName(value.toString());
						break;
					}

					case ImageCollection::Column::Type:
					{
						imageCollection->setType(static_cast<ImageData::Type>(value.toInt()));
						break;
					}

					case ImageCollection::Column::SubsamplingEnabled:
					{
						imageCollection->subsampling().setEnabled(value.toBool());
						updateTargetSize();
						break;
					}

					case ImageCollection::Column::SubsamplingRatio:
					{
						imageCollection->subsampling().setRatio(value.toFloat());
						updateTargetSize();
						break;
					}

					case ImageCollection::Column::SubsamplingFilter:
					{
						imageCollection->subsampling().setFilter(static_cast<ImageCollection::SubSampling::ImageResamplingFilter>(value.toInt()));
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
			case Qt::DisplayRole:
			{
				switch (static_cast<ImageCollection::Column>(section)) {
					case ImageCollection::Column::DatasetName:
						return "Dataset name";

					case ImageCollection::Column::ImageType:
						return "Type";

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

					case ImageCollection::Column::ToGrayscale:
						return "Grayscale";

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
	int flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

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
	TreeItem *parentItem = childItem->parentItem();

	if (parentItem == _root)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

void ImageCollectionsModel::clear()
{
	beginResetModel();
	endResetModel();
}

void ImageCollectionsModel::insert(int row, const std::vector<ImageCollection*>& imageCollections)
{
	beginInsertRows(QModelIndex(), _root->childCount(), _root->childCount() + imageCollections.size());
	{
		for (auto& imageCollection : imageCollections)
			_root->appendChild(imageCollection);
	}
	endInsertRows();
}

void ImageCollectionsModel::selectAll(const QModelIndex& index)
{
	if (index.parent() == QModelIndex()) {
		auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

		const auto shouldLoadColumn = ult(ImageCollection::Image::Column::ShouldLoad);

		for (auto& childItem : imageCollection->_children) {
			const auto row				= imageCollection->_children.indexOf(childItem);
			const auto shouldLoadIndex	= this->index(row, shouldLoadColumn, index);

			setData(shouldLoadIndex, true);
		}
	}
}

void ImageCollectionsModel::selectNone(const QModelIndex& index)
{
	if (index.parent() == QModelIndex()) {
		auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

		const auto shouldLoadColumn = ult(ImageCollection::Image::Column::ShouldLoad);

		for (auto& childItem : imageCollection->_children) {
			const auto row				= imageCollection->_children.indexOf(childItem);
			const auto shouldLoadIndex	= this->index(row, shouldLoadColumn, index);

			setData(shouldLoadIndex, false);
		}
	}
}

void ImageCollectionsModel::invertSelection(const QModelIndex& index)
{
	if (index.parent() == QModelIndex()) {
		auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

		const auto shouldLoadColumn = ult(ImageCollection::Image::Column::ShouldLoad);

		for (auto& childItem : imageCollection->_children) {
			const auto row				= imageCollection->_children.indexOf(childItem);
			const auto shouldLoadIndex	= this->index(row, shouldLoadColumn, index);
			const auto shouldLoad		= data(shouldLoadIndex, Qt::EditRole).toBool();

			setData(shouldLoadIndex, !shouldLoad);
		}
	}
}

void ImageCollectionsModel::selectPercentage(const QModelIndex& index, const float& selectionProbability)
{
	if (index.parent() == QModelIndex()) {
		auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

		const auto probability		= std::clamp(selectionProbability, 0.0f, 1.0f);
		const auto shouldLoadColumn	= ult(ImageCollection::Image::Column::ShouldLoad);

		for (auto& childItem : imageCollection->_children) {
			const auto row				= imageCollection->_children.indexOf(childItem);
			const auto shouldLoadIndex	= this->index(row, shouldLoadColumn, index);
			const auto r				= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			setData(shouldLoadIndex, r < selectionProbability);
		}
	}
}