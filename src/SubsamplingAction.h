#pragma once

#include <actions/WidgetAction.h>
#include <actions/DecimalAction.h>
#include <actions/TriggersAction.h>
#include <actions/OptionAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class SubsamplingAction : public WidgetAction
{



protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, SubsamplingAction* subsamplingAction, const std::int32_t& widgetFlags);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    SubsamplingAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin);

protected:
    ImageLoaderPlugin&  _imageLoaderPlugin;     /** Reference to parent image loader plugin */

    friend class Widget;
};