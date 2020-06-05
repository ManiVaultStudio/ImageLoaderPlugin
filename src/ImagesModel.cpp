#include "ImagesModel.h"

#include "Common.h"

#include <QDebug>
#include <QDirIterator>
#include <QPainter>

ImagesModel::ImagesModel() :
	QAbstractListModel(),
	_imageCollections()
{
}

int ImagesModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	return _imageCollections.size();
}

int ImagesModel::columnCount(const QModelIndex& parent) const
{
	return ult(Column::End) + 1;
}

QVariant ImagesModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
		return QVariant();

	const auto imageCollection = _imageCollections[index.row()];

	switch (index.column()) {
		case ult(Column::DatasetName):
			return imageCollection.datasetName(role);

		case ult(Column::NoImages):
			return imageCollection.noImages(role);

		case ult(Column::SourceSize):
			return imageCollection.sourceSize(role);

		case ult(Column::TargetSize):
			return imageCollection.targetSize(role);

		case ult(Column::SearchDir):
			return imageCollection.searchDir(role);
	}

	return QVariant();
}

QVariant ImagesModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		return columnName(static_cast<Column>(section));
	}

	return QVariant();
}

Qt::ItemFlags ImagesModel::flags(const QModelIndex& index) const
{
	int flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

	//const auto type = static_cast<Type>(_type);

	switch (static_cast<Column>(index.column())) {
		case Column::DatasetName:
		{
			flags |= Qt::ItemIsUserCheckable;

			if (index != QModelIndex())
				flags |= Qt::ItemIsEditable;

			break;
		}

		case Column::SearchDir:
		case Column::SourceSize:
		case Column::TargetSize:
			break;

		default:
			break;
	}

	return flags;
}

void ImagesModel::clear()
{
	beginResetModel();
	{
		_imageCollections.clear();
	}
	endResetModel();
}

void ImagesModel::insert(int row, const std::vector<ImageCollection>& imageCollections)
{
	beginInsertRows(QModelIndex(), _imageCollections.size(), _imageCollections.size() + imageCollections.size());
	{
		_imageCollections.insert(_imageCollections.end(), imageCollections.begin(), imageCollections.end());
	}
	endInsertRows();
}

const ImageCollection* ImagesModel::imageCollection(const int& row) const
{
	const auto colorMapIndex = index(row, 0);

	if (!colorMapIndex.isValid())
		return nullptr;

	return &_imageCollections.at(row);
}