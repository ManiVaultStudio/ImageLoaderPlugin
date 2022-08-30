#include "DataLayoutAction.h"
#include "ImageLoaderPlugin.h"

DataLayoutAction::DataLayoutAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    OptionAction(parent, "Data layout", { "Image sequence", "Image stack" }),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setEnabled(false);
    setToolTip("Determines the layout of the loaded image data");
    setPlaceHolderString("Choose data layout");
    setCurrentIndex(-1);

    connect(this, &OptionAction::currentIndexChanged, this, &DataLayoutAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &DataLayoutAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &DataLayoutAction::updateStateFromModel);
}

void DataLayoutAction::setCurrentIndexSilently(const std::int32_t& currentIndex)
{
    disconnect(this, &OptionAction::currentIndexChanged, this, nullptr);
    {
        setCurrentIndex(currentIndex);
    }
    connect(this, &OptionAction::currentIndexChanged, this, &DataLayoutAction::updateRows);
}

void DataLayoutAction::updateRows()
{
    if (getCurrentIndex() < 0)
        return;

    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::Type), getCurrentIndex());
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &DataLayoutAction::dataChanged);
}

void DataLayoutAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::Type))
            updateStateFromModel();
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &DataLayoutAction::dataChanged);
}

void DataLayoutAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0)
        setEnabled(false);

    if (numberOfSelectedRows == 1) {
        setEnabled(true);
        setCurrentIndexSilently(selectedRows.first().siblingAtColumn(ImageCollection::Column::Type).data(Qt::EditRole).toInt());
    }

    if (numberOfSelectedRows >= 2) {
        setEnabled(true);

        QSet<std::int32_t> dataLayoutTypes;

        for (const auto& selectedRow : selectedRows)
            dataLayoutTypes.insert(selectedRow.siblingAtColumn(ImageCollection::Column::Type).data(Qt::EditRole).toInt());

        if (dataLayoutTypes.count() == 1)
            setCurrentIndexSilently(dataLayoutTypes.values().first());

        if (dataLayoutTypes.count() == 2)
            setCurrentIndexSilently(-1);
    }
}
