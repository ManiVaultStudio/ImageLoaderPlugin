#include "CommonSettingsAction.h"
#include "ImageLoaderPlugin.h"

using namespace hdps::gui;

CommonSettingsAction::CommonSettingsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _scanDirectoryAction(this, "Scan directory"),
    _separateByDirectoryAction(this, "Separate by directory"),
    _filterAction(this, "Filter"),
    _nameAction(this, "Name")
{
    setText("Image collections");
}

CommonSettingsAction::Widget::Widget(QWidget* parent, CommonSettingsAction* commonSettingsAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, commonSettingsAction, widgetFlags)
{
    auto mainLayout = new QGridLayout();

    mainLayout->addWidget(commonSettingsAction->getScanDirectoryAction().createLabelWidget(this), 0, 0);
    mainLayout->addWidget(commonSettingsAction->getScanDirectoryAction().createWidget(this), 0, 1);
    mainLayout->addWidget(commonSettingsAction->getSeparateByDirectoryAction().createWidget(this), 1, 1);
    mainLayout->addWidget(commonSettingsAction->getFilterAction().createLabelWidget(this), 2, 0);
    mainLayout->addWidget(commonSettingsAction->getFilterAction().createWidget(this), 2, 1);
    mainLayout->addWidget(commonSettingsAction->getNameAction().createLabelWidget(this), 3, 0);
    mainLayout->addWidget(commonSettingsAction->getNameAction().createWidget(this), 3, 1);

    setPopupLayout(mainLayout);
}
