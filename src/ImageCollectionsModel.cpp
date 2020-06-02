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

int ImageCollectionsModel::columnCount(const QModelIndex& parent) const
{
	return ult(Column::End) + 1;
}

int ImageCollectionsModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	return _imageCollections.count();
}

QVariant ImageCollectionsModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
		return QVariant();

	const auto imageCollection = _imageCollections.at(index.row());

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

		case Qt::TextAlignmentRole:
			return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);

		default:
			break;
	}

	return QVariant();
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