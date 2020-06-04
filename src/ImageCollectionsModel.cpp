#include "ImageCollectionsModel.h"

#include "Common.h"

#include <QDebug>
#include <QDirIterator>
#include <QPainter>

ImageCollectionsModel::ImageCollectionsModel() :
	QAbstractListModel(),
	_imageCollections()
{
}

int ImageCollectionsModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	return _imageCollections.count();
}

int ImageCollectionsModel::columnCount(const QModelIndex& parent) const
{
	return ult(Column::End) + 1;
}

QVariant ImageCollectionsModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
		return QVariant();

	const auto imageCollection = _imageCollections[index.row()];

	switch (role) {
		case Qt::DisplayRole:
		{
			switch (index.column()) {
				case ult(Column::Name):
					return imageCollection.name();

				case ult(Column::SearchDir):
					return imageCollection.searchDir();

				case ult(Column::FilePath):
					return imageCollection.filePath();

				case ult(Column::SourceSize):
					return imageCollection.sourceSize();

				case ult(Column::TargetSize):
					return imageCollection.targetSize();
			}

			break;
		}
		
		case Qt::EditRole:
		{
			break;
		}

		case Qt::CheckStateRole:
		{
			switch (index.column()) {
				case ult(Column::Name):
					return Qt::Checked;

				case ult(Column::SearchDir):
				case ult(Column::FilePath):
				case ult(Column::SourceSize):
				case ult(Column::TargetSize):
					break;

				default:
					break;
			}

			break;
		}

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
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
		case Column::Name:
		{
			flags |= Qt::ItemIsUserCheckable;

			if (index != QModelIndex())
				flags |= Qt::ItemIsEditable;

			break;
		}

		case Column::SearchDir:
		case Column::FilePath:
		case Column::SourceSize:
		case Column::TargetSize:
			break;

		default:
			break;
	}

	return flags;
}

bool ImageCollectionsModel::insert(int row, const ImageCollection& imageCollection)
{
	beginInsertRows(QModelIndex(), row, row);
	{
		_imageCollections << imageCollection;
	}
	endInsertRows();

	return true;
}

const ImageCollection* ImageCollectionsModel::imageCollection(const int& row) const
{
	const auto colorMapIndex = index(row, 0);

	if (!colorMapIndex.isValid())
		return nullptr;

	return &_imageCollections.at(row);
}