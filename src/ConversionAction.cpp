#include "ConversionAction.h"
#include "ImageLoaderPlugin.h"

ConversionAction::ConversionAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    PluginTriggerPickerAction(parent, "Conversion"),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setEnabled(false);
    setToolTip("Determines the type of image data conversion");

    connect(this, &PluginTriggerPickerAction::currentPluginTriggerActionChanged, this, &ConversionAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &ConversionAction::updateStateFromModel);
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

    if (currentPluginTriggerAction == nullptr || currentPluginTriggerAction->getSha() == "")
        return;

    for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
        _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::Conversion), currentPluginTriggerAction->getSha());
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
