#pragma once

#include <actions/WidgetAction.h>
#include <actions/DirectoryPickerAction.h>
#include <actions/ToggleAction.h>
#include <actions/StringAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class CommonSettingsAction : public WidgetAction
{
protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, CommonSettingsAction* commonSettingsAction, const std::int32_t& widgetFlags);
    };

public:

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    CommonSettingsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin);

public:

    DirectoryPickerAction& getScanDirectoryAction() { return _scanDirectoryAction; }
    ToggleAction& getSeparateByDirectoryAction() { return _separateByDirectoryAction; }
    StringAction& getFilterAction() { return _filterAction; }
    StringAction& getNameAction() { return _nameAction; }

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;
    DirectoryPickerAction   _scanDirectoryAction;
    ToggleAction            _separateByDirectoryAction;
    StringAction            _filterAction;
    StringAction            _nameAction;

    friend class Widget;
};