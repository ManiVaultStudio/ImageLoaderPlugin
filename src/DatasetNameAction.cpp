#include "DatasetNameAction.h"
#include "ImageLoaderPlugin.h"

DatasetNameAction::DatasetNameAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    StringAction(parent, "Dataset name"),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setEnabled(false);
    setToolTip("Determines the dataset name in the HDPS data model");
    setPlaceHolderString("Enter dataset name...");

    connect(this, &StringAction::stringChanged, this, &DatasetNameAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &DatasetNameAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &DatasetNameAction::updateStateFromModel);

    updateStateFromModel();
}

void DatasetNameAction::setDatasetNameSilently(const QString& datasetName)
{
    disconnect(this, &DatasetNameAction::stringChanged, this, nullptr);
    {
        setString(datasetName);
    }
    connect(this, &DatasetNameAction::stringChanged, this, &DatasetNameAction::updateRows);
}

void DatasetNameAction::updateRows()
{
    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::DatasetName), getString());
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &DatasetNameAction::dataChanged);
}

void DatasetNameAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::DatasetName))
        updateStateFromModel();
}

void DatasetNameAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0) {
        setEnabled(false);
        setDatasetNameSilently("");
    }

    if (numberOfSelectedRows == 1) {
        setEnabled(true);
        setDatasetNameSilently(selectedRows.first().siblingAtColumn(ImageCollection::Column::DatasetName).data(Qt::EditRole).toString());
    }

    if (numberOfSelectedRows >= 2) {
        setEnabled(false);
        setDatasetNameSilently("");
    }
}
