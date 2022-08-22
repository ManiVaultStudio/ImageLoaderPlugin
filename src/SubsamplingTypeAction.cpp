#include "SubsamplingTypeAction.h"
#include "ImageLoaderPlugin.h"

SubsamplingTypeAction::SubsamplingTypeAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    OptionAction(parent, "Subsampling", ImageCollection::SubSampling::types.values()),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setEnabled(false);
    setToolTip("Determines the type of image data subsampling");
    setPlaceHolderString("Pick...");
    setCurrentIndex(-1);

    connect(this, &OptionAction::currentIndexChanged, this, &SubsamplingTypeAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &SubsamplingTypeAction::updateStateFromModel);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &SubsamplingTypeAction::updateStateFromModel);

    updateStateFromModel();
}

void SubsamplingTypeAction::setCurrentIndexSilently(const std::int32_t& currentIndex)
{
    disconnect(this, &OptionAction::currentIndexChanged, this, nullptr);
    {
        setCurrentIndex(currentIndex);
    }
    connect(this, &OptionAction::currentIndexChanged, this, &SubsamplingTypeAction::updateRows);
}

void SubsamplingTypeAction::updateRows()
{
    if (getCurrentIndex() < 0)
        return;

    for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
        _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingType), getCurrentIndex());
}

void SubsamplingTypeAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0)
        setEnabled(false);

    if (numberOfSelectedRows == 1) {
        setEnabled(true);
        setCurrentIndexSilently(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingType).data(Qt::EditRole).toInt());
    }

    if (numberOfSelectedRows >= 2) {
        setEnabled(true);

        QSet<std::int32_t> subsamplingTypes;

        for (const auto& selectedRow : selectedRows)
            subsamplingTypes.insert(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingType).data(Qt::EditRole).toInt());

        if (subsamplingTypes.count() == 1)
            setCurrentIndexSilently(subsamplingTypes.values().first());

        if (subsamplingTypes.count() == 2)
            setCurrentIndexSilently(-1);
    }
}
