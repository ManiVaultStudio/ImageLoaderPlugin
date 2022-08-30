#include "ConversionAction.h"
#include "ImageLoaderPlugin.h"

ConversionAction::ConversionAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    PluginTriggerPickerAction(parent, "Conversion"),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setEnabled(false);
    setToolTip("Determines the type of image data conversion");

    connect(this, &PluginTriggerPickerAction::currentPluginTriggerActionChanged, this, &ConversionAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &ConversionAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &ConversionAction::updateStateFromModel);

    updateStateFromModel();
}

void ConversionAction::setCurrentPluginTriggerActionSilently(const QString& currentPluginTriggerSha)
{
    disconnect(this, &PluginTriggerPickerAction::currentPluginTriggerActionChanged, this, nullptr);
    {
        setCurrentPluginTriggerAction(currentPluginTriggerSha);
    }
    connect(this, &PluginTriggerPickerAction::currentPluginTriggerActionChanged, this, &ConversionAction::updateRows);
}

void ConversionAction::updateRows()
{
    auto currentPluginTriggerAction = getCurrentPluginTriggerAction();

    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::Conversion), currentPluginTriggerAction == nullptr ? "" : currentPluginTriggerAction->getSha());
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &ConversionAction::dataChanged);
}

void ConversionAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::Conversion))
        updateStateFromModel();
}

void ConversionAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0) {
        setEnabled(false);
        setCurrentPluginTriggerActionSilently("");
    }

    if (numberOfSelectedRows == 1) {
        setCurrentPluginTriggerActionSilently(selectedRows.first().siblingAtColumn(ImageCollection::Column::Conversion).data(Qt::EditRole).toString());
        setEnabled(true);
    }

    if (numberOfSelectedRows >= 2) {
        setEnabled(true);

        QSet<QString> conversions;

        for (const auto& selectedRow : selectedRows)
            conversions.insert(selectedRow.siblingAtColumn(ImageCollection::Column::Conversion).data(Qt::EditRole).toString());
        
        setCurrentPluginTriggerActionSilently((conversions.isEmpty() || conversions.count() == 2) ? "" : conversions.values().first());
    }
}
