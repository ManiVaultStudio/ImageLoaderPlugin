#pragma once

#include <actions/WidgetAction.h>
#include <actions/DirectoryPickerAction.h>
#include <actions/ToggleAction.h>

class ImageLoaderPlugin;

using namespace mv::gui;

class ScanAction : public WidgetAction
{
protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, ScanAction* commonSettingsAction, const std::int32_t& widgetFlags);
    };

public:

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    ScanAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin);

public:

    DirectoryPickerAction& getScanDirectoryAction() { return _scanDirectoryAction; }
    ToggleAction& getSeparateByDirectoryAction() { return _separateByDirectoryAction; }

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;
    DirectoryPickerAction   _scanDirectoryAction;
    ToggleAction            _separateByDirectoryAction;

};