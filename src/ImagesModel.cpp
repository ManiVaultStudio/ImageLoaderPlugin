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
	auto tooltip = [](const QString& title, const QString& description) {
		return QString("<html><head/><body><p><span style='font-weight: 600;'>%1</span><br/>%2</p></body></html>").arg(title, description);
	};

	if (orientation == Qt::Horizontal) {
		switch (role)
		{
			case Qt::DisplayRole:
			{
				switch (static_cast<Column>(section)) {
					case Column::ShouldLoad:
						return "";

					case Column::FileName:
						return "Filename";

					case Column::DimensionName:
						return "Dimension name";

					case Column::FilePath:
						return "File path";

					default:
						return QString();
				}

				break;
			}

			case Qt::ToolTipRole:
			{
				switch (static_cast<Column>(section)) {
					case Column::ShouldLoad:
						return tooltip("Load", "Whether to load the image or not");

					case Column::FileName:
						return tooltip("Filename", "Name of the image file");

					case Column::DimensionName:
						return tooltip("Dimension name", "Name of the dimension in the high-dimensional dataset, click to edit");

					case Column::FilePath:
						return tooltip("File path", "The absolute file path of the image");

					default:
						return QString();
				}

				break;
			}
		}
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

	_imageCollection = imageCollection;

	if (imageCollection != nullptr) {
		beginInsertRows(QModelIndex(), 0, imageCollection->noImages(Qt::EditRole).toInt());
		endInsertRows();
	}
}