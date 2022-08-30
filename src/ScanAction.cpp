#include "ScanAction.h"
#include "ImageLoaderPlugin.h"

using namespace hdps::gui;

ScanAction::ScanAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _scanDirectoryAction(this, "Directory"),
    _separateByDirectoryAction(this, "Separate by directory")
{
    setText("Scan");
    
    connect(&_scanDirectoryAction, &DirectoryPickerAction::directoryChanged, this, [this](const QString& directory) -> void {
        _imageLoaderPlugin.getImageCollectionScanner().setDirectory(directory);
    });

    connect(&_separateByDirectoryAction, &ToggleAction::toggled, this, [this](bool toggled) -> void {
        _imageLoaderPlugin.getImageCollectionScanner().setSeparateByDirectory(toggled);
    });

    _scanDirectoryAction.setSettingsPrefix(&imageLoaderPlugin, "ScanDirectory");
    _separateByDirectoryAction.setSettingsPrefix(&imageLoaderPlugin, "SeparateByDirectory");
}

ScanAction::Widget::Widget(QWidget* parent, ScanAction* commonSettingsAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, commonSettingsAction, widgetFlags)
{
    auto mainLayout = new QGridLayout();

    mainLayout->addWidget(commonSettingsAction->getScanDirectoryAction().createLabelWidget(this), 0, 0);
    mainLayout->addWidget(commonSettingsAction->getScanDirectoryAction().createWidget(this), 0, 1);
    mainLayout->addWidget(commonSettingsAction->getSeparateByDirectoryAction().createWidget(this), 1, 1);

    setPopupLayout(mainLayout);

    layout()->setContentsMargins(0, 0, 0, 0);
}
