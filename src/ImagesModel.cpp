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
		case ult(Column::Name):
			return image->name(role);

		case ult(Column::FilePath):
			return image->filePath(role);

		case ult(Column::ShouldLoad):
			return image->shouldLoad(role);
	}

	return QVariant();
}

bool ImagesModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	auto image = _imageCollection->image(index.row());
	
	const auto column = static_cast<Column>(index.column());

	switch (role)
	{
		case Qt::CheckStateRole:
		{
			switch (column) {
				case Column::Name:
				{
					image->setShouldLoad(value.toBool());
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

	//const auto type = static_cast<Type>(_type);

	switch (static_cast<Column>(index.column())) {
		case Column::Name:
		{
			flags |= Qt::ItemIsUserCheckable;

			if (index != QModelIndex())
				flags |= Qt::ItemIsEditable;

			break;
		}

		case Column::FilePath:
		case Column::ShouldLoad:
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