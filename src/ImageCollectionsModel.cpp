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

		case ult(Column::SearchDir):
			return imageCollection.searchDir(role);

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
					break;
				}

				case Column::SubsamplingRatio:
				{
					imageCollection.subsampling().setRatio(value.toFloat());

					const auto sourceSize = imageCollection.sourceSize(Qt::EditRole).toSize();

					imageCollection.setTargetSize(value.toFloat() * sourceSize);

					affectedIndices << index.siblingAtColumn(ult(Column::TargetSize));
					affectedIndices << index.siblingAtColumn(ult(Column::TargetWidth));
					affectedIndices << index.siblingAtColumn(ult(Column::TargetHeight));
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
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		return columnName(static_cast<Column>(section));
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

		case Column::SearchDir:
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