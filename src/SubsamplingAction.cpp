#include "SubsamplingAction.h"
#include "ImageLoaderPlugin.h"

#include <QGridLayout>
#include <QHBoxLayout>

using namespace hdps::gui;

const QMap<SubsamplingAction::Scale, TriggersAction::Trigger> SubsamplingAction::triggers = QMap<SubsamplingAction::Scale, TriggersAction::Trigger>({
    { SubsamplingAction::Perc5, TriggersAction::Trigger("5.0%", "Scale down to 5%") },
    { SubsamplingAction::Perc10, TriggersAction::Trigger("10.0%", "Scale down to 10%") },
    { SubsamplingAction::Perc25, TriggersAction::Trigger("25.0%", "Scale down to 25%") },
    { SubsamplingAction::Perc50, TriggersAction::Trigger("50.0%", "Scale down to 50%") },
    { SubsamplingAction::Perc75, TriggersAction::Trigger("75.0%", "Scale down to 75%") }
});

const QMap<SubsamplingAction::Scale, float> SubsamplingAction::scaleFactors = QMap<SubsamplingAction::Scale, float>({
    { SubsamplingAction::Perc5, 5.0f },
    { SubsamplingAction::Perc10, 10.0f },
    { SubsamplingAction::Perc25, 25.0f },
    { SubsamplingAction::Perc50, 50.0f },
    { SubsamplingAction::Perc75, 75.0f }
});

SubsamplingAction::SubsamplingAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _ratioAction(this, "Ratio", 1.0f, 100.0f, 50.0f, 50.0f, 1),
    _defaultRatiosAction(this, "Default ratios", triggers.values().toVector()),
    _filterTypeAction(this, "Filter type", { "Box", "Bilinear", "BSpline", "Bicubic", "Catmull Rom", "Lanczos" })
{
    setText("Subsampling");
    setCheckable(true);

    _ratioAction.setSuffix("%");

    connect(&_defaultRatiosAction, &TriggersAction::triggered, this, [this](std::int32_t triggerIndex) {
        _ratioAction.setValue(scaleFactors.values().at(triggerIndex));
    });
}

SubsamplingAction::Widget::Widget(QWidget* parent, SubsamplingAction* subsamplingAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, subsamplingAction, widgetFlags)
{
    auto mainLayout = new QGridLayout();

    mainLayout->addWidget(subsamplingAction->getRatioAction().createLabelWidget(this), 0, 0);
    
    auto ratioLayout = new QHBoxLayout();

    ratioLayout->setContentsMargins(0, 0, 0, 0);
    ratioLayout->addWidget(subsamplingAction->getRatioAction().createWidget(this), 1);
    ratioLayout->addWidget(subsamplingAction->getDefaultRatiosAction().createWidget(this));

    mainLayout->addLayout(ratioLayout, 0, 1);

    mainLayout->addWidget(subsamplingAction->getFilterTypeAction().createLabelWidget(this), 1, 0);
    mainLayout->addWidget(subsamplingAction->getFilterTypeAction().createWidget(this), 1, 1);

    setPopupLayout(mainLayout);
}
