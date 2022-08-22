#include "DataLayoutAction.h"
#include "ImageLoaderPlugin.h"

DataLayoutAction::DataLayoutAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    OptionAction(parent, "Data layout", { "Image sequence", "Image stack" }),
    _imageLoaderPlugin(imageLoaderPlugin),
    _indices()
{
    setEnabled(false);
    setToolTip("Determines the layout of the loaded image data");
    setPlaceHolderString("Choose data layout");
    setCurrentIndex(-1);

    connect(this, &OptionAction::currentIndexChanged, this, &DataLayoutAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows         = _imageLoaderPlugin.getSelectedImageCollectionIndices();
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
    });
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

    for (const auto& selectedRow : _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows())
        _imageLoaderPlugin.getImageCollectionsModel().setData(_imageLoaderPlugin.getImageCollectionsFilterModel().mapToSource(selectedRow).siblingAtColumn(ImageCollection::Column::Type), getCurrentIndex());
}
