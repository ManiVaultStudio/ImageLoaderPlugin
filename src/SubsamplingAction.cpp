#include "SubsamplingAction.h"
#include "ImageLoaderPlugin.h"

#include <QGridLayout>
#include <QHBoxLayout>

using namespace hdps::gui;

SubsamplingAction::SubsamplingAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _typeAction(this, imageLoaderPlugin),
    _ratioAction(this, imageLoaderPlugin),
    _filterTypeAction(this, imageLoaderPlugin),
    _numberOfLevelsAction(this, imageLoaderPlugin)
{
    setText("Subsampling");
    setCheckable(true);

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = _imageLoaderPlugin.getSelectedRows();
        const auto numberOfSelectedRows = selectedRows.count();

        if (numberOfSelectedRows == 0)
            setEnabled(false);

        if (numberOfSelectedRows == 1) {
            //_subsamplingEnabledAction.setEnabled(true);
            //_subsamplingEnabledAction.setChecked(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingEnabled).data(Qt::EditRole).toBool());
            //setSubsamplingFilter(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingFilter).data(Qt::EditRole).toInt());
        }

        if (numberOfSelectedRows >= 2) {
            //QSet<std::int32_t> subsamplingEnabled;

            //for (const auto& selectedRow : selectedRows)
            //    subsamplingEnabled.insert(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingEnabled).data(Qt::EditRole).toBool());

            //if (subsamplingEnabled.count() == 1)
            //    setSubsamplingFilter(subsamplingEnabled.values().first());

            //if (subsamplingEnabled.count() >= 2)
            //    _subsamplingEnabledAction.setIndeterminate(true);

            //QSet<std::int32_t> subsamplingFilterTypes;

            //for (const auto& selectedRow : selectedRows)
            //    subsamplingFilterTypes.insert(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingFilter).data(Qt::EditRole).toInt());

            //if (subsamplingFilterTypes.count() == 1)
            //    setSubsamplingFilter(subsamplingFilterTypes.values().first());

            //if (subsamplingFilterTypes.count() >= 2)
            //    setSubsamplingFilter(-1);

            //_subsamplingEnabledAction.setEnabled(true);
        }
    });

    /*
    const auto updateSubsamplingActions = [this]() -> void {
        _subsamplingRatioAction.setEnabled(_subsamplingEnabledAction.isChecked());
        _subsamplingDefaultRatiosAction.setEnabled(_subsamplingEnabledAction.isChecked());
        _subsamplingFilterTypeAction.setEnabled(_subsamplingEnabledAction.isChecked());
    };

    connect(&_subsamplingEnabledAction, &ToggleAction::toggled, this, [this, updateSubsamplingActions](bool toggled) -> void {
        updateSubsamplingActions();

        const auto selectedImageCollectionIndices = _imageLoaderPlugin.getSelectedImageCollectionIndices();

        for (const auto& selectedImageCollectionIndex : selectedImageCollectionIndices)
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedImageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingEnabled)), toggled);
    });

    connect(&_subsamplingRatioAction, &DecimalAction::valueChanged, this, [this](const float& value) -> void {
        const auto selectedImageCollectionIndices = _imageLoaderPlugin.getSelectedImageCollectionIndices();

        for (const auto& selectedImageCollectionIndex : selectedImageCollectionIndices)
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedImageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 0.01f * value);
    });

    connect(&_subsamplingFilterTypeAction, &OptionAction::currentIndexChanged, this, [this](const std::int32_t& currentIndex) -> void {
        const auto selectedImageCollectionIndices = _imageLoaderPlugin.getSelectedImageCollectionIndices();

        for (const auto& selectedImageCollectionIndex : selectedImageCollectionIndices)
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedImageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingFilter)), currentIndex);
    });

    updateSubsamplingActions();
    */
}

SubsamplingAction::Widget::Widget(QWidget* parent, SubsamplingAction* subsamplingAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, subsamplingAction, widgetFlags)
{
}
