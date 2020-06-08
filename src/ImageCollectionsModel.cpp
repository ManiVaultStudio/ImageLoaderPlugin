#include "ImageCollectionsModel.h"

#include "Common.h"

#include <QDebug>
#include <QDirIterator>
#include <QPainter>

ImageCollectionsModel::ImageCollectionsModel() :
	QAbstractListModel(),
	_imageCollections(),
	_selectionModel(this)
{
}

int ImageCollectionsModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	return _imageCollections.size();
}

int ImageCollectionsModel::columnCount(const QModelIndex& parent) const
{
	return ult(Column::End) + 1;
}

QVariant ImageCollectionsModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
		return QVariant();

	auto imageCollection = _imageCollections[index.row()];

	switch (index.column()) {
		case ult(Column::DatasetName):
			return imageCollection.datasetName(role);

		case ult(Column::ImageType):
			return imageCollection.imageType(role);

		case ult(Column::NoImages):
			return imageCollection.noImages(role);

		case ult(Column::NoSelectedImages):
			return imageCollection.noSelectedImages(role);

		case ult(Column::ToGrayscale):
			return imageCollection.toGrayscale(role);

		case ult(Column::SourceSize):
			return imageCollection.sourceSize(role);

		case ult(Column::TargetSize):
			return imageCollection.targetSize(role);

		case ult(Column::TargetWidth):
			return imageCollection.targetWidth(role);

		case ult(Column::TargetHeight):
			return imageCollection.targetheight(role);

		case ult(Column::Directory):
			return imageCollection.directory(role);

		case ult(Column::Type):
			return imageCollection.type(role);

		case ult(Column::SubsamplingEnabled):
			return imageCollection.subsampling().enabled(role);

		case ult(Column::SubsamplingRatio):
			return imageCollection.subsampling().ratio(role);

		case ult(Column::SubsamplingFilter):
			return imageCollection.subsampling().filter(role);
	}

	return QVariant();
}

bool ImageCollectionsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	QModelIndexList affectedIndices;

	affectedIndices << index;

	auto& imageCollection = _imageCollections[index.row()];

	const auto column = static_cast<Column>(index.column());

	auto updateTargetSize = [&index, &imageCollection, &affectedIndices]() {
		const auto subsamplingEnabled	= imageCollection.subsampling().enabled(Qt::EditRole).toBool();
		const auto subsamplingRatio		= imageCollection.subsampling().ratio(Qt::EditRole).toFloat();
		const auto sourceSize			= imageCollection.sourceSize(Qt::EditRole).toSize();

		imageCollection.setTargetSize(subsamplingEnabled ? subsamplingRatio * sourceSize : sourceSize);

		affectedIndices << index.siblingAtColumn(ult(Column::TargetSize));
		affectedIndices << index.siblingAtColumn(ult(Column::TargetWidth));
		affectedIndices << index.siblingAtColumn(ult(Column::TargetHeight));
	};

	switch (role)
	{
		case Qt::EditRole:
		{
			switch (column) {
				case Column::DatasetName:
				{
					imageCollection.setDatasetName(value.toString());
					break;
				}

				case Column::Type:
				{
					imageCollection.setType(static_cast<ImageData::Type>(value.toInt()));
					break;
				}

				case Column::SubsamplingEnabled:
				{
					imageCollection.subsampling().setEnabled(value.toBool());
					updateTargetSize();
					break;
				}

				case Column::SubsamplingRatio:
				{
					imageCollection.subsampling().setRatio(value.toFloat());
					updateTargetSize();
					break;
				}

				case Column::SubsamplingFilter:
				{
					imageCollection.subsampling().setFilter(static_cast<ImageCollection::SubSampling::ImageResamplingFilter>(value.toInt()));
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
				case Column::ToGrayscale:
				{
					imageCollection.setToGrayscale(value.toBool());
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
				switch (static_cast<Column>(section)) {
					case Column::DatasetName:
						return "Dataset name";

					case Column::ImageType:
						return "Type";

					case Column::NoImages:
						return "#Images";

					case Column::NoSelectedImages:
						return "#Selected images";

					case Column::SourceSize:
						return "Source size";

					case Column::TargetSize:
						return "Target size";

					case Column::TargetWidth:
						return "Width";

					case Column::TargetHeight:
						return "Height";

					case Column::Directory:
						return "Directory";

					case Column::Type:
						return "Load as";

					case Column::SubsamplingEnabled:
						return "Subsampling enabled";

					case Column::SubsamplingRatio:
						return "Subsampling ratio";

					case Column::SubsamplingFilter:
						return "Subsampling filter";

					case Column::ToGrayscale:
						return "Convert to grayscale";

					default:
						return QString();
				}

				break;
			}

			case Qt::ToolTipRole:
			{
				switch (static_cast<Column>(section)) {
					case Column::DatasetName:
						return tooltip("Dataset name", "The name of the high-dimensional dataset");

					case Column::ImageType:
						return tooltip("Type", "The type of images in the scanned image collection");

					case Column::NoImages:
						return tooltip("#Images", "The number of images in the collection");

					case Column::NoSelectedImages:
						return tooltip("#Selected images", "The number of selected images in the collection");

					case Column::SourceSize:
						return tooltip("Source size", "The size of the images on disk");

					case Column::TargetSize:
						return tooltip("Target size", "The size of the images when loaded as high-dimensional data");

					case Column::TargetWidth:
						return tooltip("Width", "The width of the images when loaded as high-dimensional data");

					case Column::TargetHeight:
						return tooltip("Height", "The height of the images when loaded as high-dimensional data");

					case Column::Directory:
						return tooltip("Directory", "The top-level directory where the images were found");

					case Column::Type:
						return tooltip("Load as", "How to interpret the images as high-dimensional data");

					case Column::SubsamplingEnabled:
						return tooltip("Subsampling enabled", "Whether images are sub-sampled when loaded as high-dimensional data");

					case Column::SubsamplingRatio:
						return tooltip("Subsampling ratio", "The amount of subsampling");

					case Column::SubsamplingFilter:
						return tooltip("Subsampling filter", "The subsampling filter to use");

					case Column::ToGrayscale:
						return tooltip("Convert to grayscale", "Whether all image channels are combined in to one (grayscale)");

					default:
						return QString();
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

	switch (static_cast<Column>(index.column())) {
		case Column::DatasetName:
		{
			flags |= Qt::ItemIsEditable;
			break;
		}

		case Column::ToGrayscale:
		{
			flags |= Qt::ItemIsUserCheckable;

			if (index != QModelIndex())
				flags |= Qt::ItemIsEditable;

			break;
		}

		case Column::Directory:
		case Column::SourceSize:
		case Column::TargetSize:
		case Column::TargetWidth:
		case Column::TargetHeight:
			break;

		default:
			break;
	}

	return flags;
}

void ImageCollectionsModel::clear()
{
	beginResetModel();
	{
		_imageCollections.clear();
	}
	endResetModel();
}

void ImageCollectionsModel::insert(int row, const std::vector<ImageCollection>& imageCollections)
{
	beginInsertRows(QModelIndex(), _imageCollections.size(), _imageCollections.size() + imageCollections.size());
	{
		_imageCollections.insert(_imageCollections.end(), imageCollections.begin(), imageCollections.end());
	}
	endInsertRows();
}

const ImageCollection* ImageCollectionsModel::imageCollection(const int& row) const
{
	const auto colorMapIndex = index(row, 0);

	if (!colorMapIndex.isValid())
		return nullptr;

	return &_imageCollections.at(row);
}