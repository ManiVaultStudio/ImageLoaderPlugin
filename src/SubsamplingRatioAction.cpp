#include "SubsamplingRatioAction.h"
#include "ImageLoaderPlugin.h"

#include <QHBoxLayout>

using namespace mv::gui;

const QMap<SubsamplingRatioAction::Ratio, TriggersAction::Trigger> SubsamplingRatioAction::triggers = QMap<SubsamplingRatioAction::Ratio, TriggersAction::Trigger>({
    { SubsamplingRatioAction::Perc5, TriggersAction::Trigger("5.0%", "Scale down to 5%") },
    { SubsamplingRatioAction::Perc10, TriggersAction::Trigger("10.0%", "Scale down to 10%") },
    { SubsamplingRatioAction::Perc25, TriggersAction::Trigger("25.0%", "Scale down to 25%") },
    { SubsamplingRatioAction::Perc50, TriggersAction::Trigger("50.0%", "Scale down to 50%") },
    { SubsamplingRatioAction::Perc75, TriggersAction::Trigger("75.0%", "Scale down to 75%") }
});

const QMap<SubsamplingRatioAction::Ratio, float> SubsamplingRatioAction::defaultRatios = QMap<SubsamplingRatioAction::Ratio, float>({
    { SubsamplingRatioAction::Perc5, 5.0f },
    { SubsamplingRatioAction::Perc10, 10.0f },
    { SubsamplingRatioAction::Perc25, 25.0f },
    { SubsamplingRatioAction::Perc50, 50.0f },
    { SubsamplingRatioAction::Perc75, 75.0f }
});

SubsamplingRatioAction::SubsamplingRatioAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent, "Subsampling Ratio"),
    _imageLoaderPlugin(imageLoaderPlugin),
    _ratioAction(this, "Ratio", 1.0f, 100.0f, 50.0f, 1),
    _defaultRatiosAction(this, "Default ratios", triggers.values().toVector())
{
    setCheckable(true);

    _ratioAction.setToolTip("Subsampling ratio");
    _ratioAction.setSuffix("%");

    connect(&_ratioAction, &DecimalAction::valueChanged, this, &SubsamplingRatioAction::updateRows);

    connect(&_defaultRatiosAction, &TriggersAction::triggered, this, [this](std::int32_t triggerIndex) {
        _ratioAction.setValue(defaultRatios.values().at(triggerIndex));
    });

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &SubsamplingRatioAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &SubsamplingRatioAction::updateStateFromModel);

    updateStateFromModel();
}

void SubsamplingRatioAction::updateRows()
{
    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingRatio), 0.01f * _ratioAction.getValue());
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &SubsamplingRatioAction::dataChanged);
}

void SubsamplingRatioAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::SubsamplingType) && isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::SubsamplingRatio))
        updateStateFromModel();
}

void SubsamplingRatioAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0)
        setEnabled(false);

    if (numberOfSelectedRows >= 1) {
        setEnabled(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingType).data(Qt::EditRole).toInt() == static_cast<std::int32_t>(ImageCollection::SubSampling::Type::Resample));
        _ratioAction.setValue(100.0f * selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingRatio).data(Qt::EditRole).toFloat());
    }
}

SubsamplingRatioAction::Widget::Widget(QWidget* parent, SubsamplingRatioAction* subsamplingRatioAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, subsamplingRatioAction, widgetFlags)
{
    auto layout = new QHBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(subsamplingRatioAction->_ratioAction.createWidget(this), 1);
    layout->addWidget(subsamplingRatioAction->_defaultRatiosAction.createWidget(this));

    setLayout(layout);
}
