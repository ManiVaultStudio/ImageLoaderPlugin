#include "SubsamplingFilterTypeAction.h"
#include "ImageLoaderPlugin.h"

SubsamplingFilterTypeAction::SubsamplingFilterTypeAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    OptionAction(parent, "Filter type", ImageCollection::SubSampling::filters.values()),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setEnabled(false);
    setToolTip("The type of subsampling filter");
    setPlaceHolderString("Pick...");
    setCurrentIndex(-1);

    connect(this, &OptionAction::currentIndexChanged, this, &SubsamplingFilterTypeAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &SubsamplingFilterTypeAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &SubsamplingFilterTypeAction::updateStateFromModel);

    updateStateFromModel();
}

void SubsamplingFilterTypeAction::setCurrentIndexSilently(const std::int32_t& currentIndex)
{
    disconnect(this, &OptionAction::currentIndexChanged, this, nullptr);
    {
        setCurrentIndex(currentIndex);
    }
    connect(this, &OptionAction::currentIndexChanged, this, &SubsamplingFilterTypeAction::updateRows);
}

void SubsamplingFilterTypeAction::updateRows()
{
    if (getCurrentIndex() < 0)
        return;

    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingFilter), getCurrentIndex());
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &SubsamplingFilterTypeAction::dataChanged);
}

void SubsamplingFilterTypeAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::SubsamplingType) || isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::SubsamplingFilter))
        updateStateFromModel();
}

void SubsamplingFilterTypeAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0)
        setEnabled(false);

    if (numberOfSelectedRows >= 1) {
        setEnabled(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingType).data(Qt::EditRole).toInt() == static_cast<std::int32_t>(ImageCollection::SubSampling::Type::Resample));

        QSet<std::int32_t> filterTypes;

        for (const auto& selectedRow : selectedRows)
            filterTypes.insert(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingFilter).data(Qt::EditRole).toInt());

        if (filterTypes.count() == 1)
            setCurrentIndexSilently(filterTypes.values().first());

        if (filterTypes.count() >= 2)
            setCurrentIndexSilently(-1);
    }
}
