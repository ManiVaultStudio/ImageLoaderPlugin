#pragma once

#include <actions/WidgetAction.h>
#include <actions/ToggleAction.h>
#include <actions/StringAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class GroupDataAction : public ToggleAction
{
protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, GroupDataAction* groupDataAction, const std::int32_t& widgetFlags);
    };

public:

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    GroupDataAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void updateStateFromModel();

private:
    ImageLoaderPlugin&  _imageLoaderPlugin;
    ToggleAction        _enabledAction;
    StringAction        _nameAction;

    friend class Widget;
};