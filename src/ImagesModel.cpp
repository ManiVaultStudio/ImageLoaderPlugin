#include "ImagesModel.h"

#include "Common.h"

#include <QDebug>
#include <QDirIterator>
#include <QPainter>

ImagesModel::ImagesModel() :
	QAbstractListModel(),
	_imageCollection(nullptr),
	_selectionModel(this)
{
}

int ImagesModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	if (_imageCollection == nullptr)
		return 0;

	return _imageCollection->noImages(Qt::EditRole).toInt();
}

int ImagesModel::columnCount(const QModelIndex& parent) const
{
	return ult(Column::End) + 1;
}

QVariant ImagesModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
		return QVariant();

	const auto image = _imageCollection->image(index.row());

	switch (index.column()) {
		case ult(Column::ShouldLoad):
			return image->shouldLoad(role);

		case ult(Column::FileName):
			return image->fileName(role);

		case ult(Column::DimensionName):
			return image->dimensionName(role);

		case ult(Column::FilePath):
			return image->filePath(role);
	}

	return QVariant();
}

bool ImagesModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	QModelIndexList affectedIndices;

	affectedIndices << index;

	auto image = _imageCollection->image(index.row());
	
	const auto column = static_cast<Column>(index.column());

	switch (role)
	{
		case Qt::EditRole:
		{
			switch (column) {
				case Column::ShouldLoad:
				case Column::FileName:
					break;

				case Column::DimensionName:
				{
					image->setDimensionName(value.toString());
					break;
				}

				case Column::FilePath:
					break;

				default:
					break;
			}

			break;
		}

		case Qt::CheckStateRole:
		{
			switch (column) {
				case Column::ShouldLoad:
				{
					image->setShouldLoad(value.toBool());
					break;
				}

				case Column::FileName:
				case Column::DimensionName:
				case Column::FilePath:
					break;

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

	switch (static_cast<Column>(index.column())) {
		case Column::ShouldLoad:
		{
			flags |= Qt::ItemIsUserCheckable;

			if (index != QModelIndex())
				flags |= Qt::ItemIsEditable;

			break;
		}

		case Column::FileName:
			break;

		case Column::DimensionName:
		{
			flags |= Qt::ItemIsEditable;
			break;
		}

		case Column::FilePath:
			break;

		default:
			break;
	}

	return flags;
}

void ImagesModel::setImageCollection(ImageCollection* imageCollection)
{
	beginResetModel();
	endResetModel();

	beginInsertRows(QModelIndex(), 0, imageCollection->noImages(Qt::EditRole).toInt());
	{
		_imageCollection = imageCollection;
	}
	endInsertRows();
}