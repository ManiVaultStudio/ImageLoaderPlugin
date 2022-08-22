#pragma once

#include "SubsamplingTypeAction.h"
#include "SubsamplingRatioAction.h"
#include "SubsamplingFilterTypeAction.h"
#include "NumberOfLevelsActions.h"

#include <actions/WidgetAction.h>
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
    SubsamplingAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

public:
    SubsamplingTypeAction& getTypeAction() { return _typeAction; }
    SubsamplingRatioAction& getRatioAction() { return _ratioAction; }
    SubsamplingFilterTypeAction& getFilterTypeAction() { return _filterTypeAction; }
    NumberOfLevelsActions& getNumberOfLevelsAction() { return _numberOfLevelsAction; }

private:
    ImageLoaderPlugin&              _imageLoaderPlugin;         /** Reference to parent image loader plugin */
    SubsamplingTypeAction           _typeAction;                /** Subsampling type action */
    SubsamplingRatioAction          _ratioAction;               /** Subsampling ratio action */
    SubsamplingFilterTypeAction     _filterTypeAction;          /** Subsampling filter type action */
    NumberOfLevelsActions           _numberOfLevelsAction;      /** Subsampling number of levels action */

    friend class Widget;
};