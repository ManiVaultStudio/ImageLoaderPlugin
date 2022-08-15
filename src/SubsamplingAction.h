#pragma once

#include <actions/WidgetAction.h>
#include <actions/DecimalAction.h>
#include <actions/TriggersAction.h>
#include <actions/OptionAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class SubsamplingAction : public WidgetAction
{

public:

    /** Scale options */
    enum Scale {
        Perc5,      /** Scale by 5% */
        Perc10,     /** Scale by 10% */
        Perc25,     /** Scale by 25% */
        Perc50,     /** Scale by 50% */
        Perc75      /** Scale by 75% */
    };

    static const QMap<Scale, TriggersAction::Trigger> triggers;     /** Maps scale enum to trigger */
    static const QMap<Scale, float> scaleFactors;                   /** Maps scale enum to scale factor */

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

public: // Action getters

    DecimalAction& getRatioAction() { return _ratioAction; }
    TriggersAction& getDefaultRatiosAction() { return _defaultRatiosAction; }
    OptionAction& getFilterTypeAction() { return _filterTypeAction; }

protected:
    ImageLoaderPlugin&  _imageLoaderPlugin;     /** Reference to parent image loader plugin */
    DecimalAction       _ratioAction;           /** Subsampling ratio action */
    TriggersAction      _defaultRatiosAction;   /** Default subsampling ratios action */
    OptionAction        _filterTypeAction;      /** Subsampling filter type action */

    friend class Widget;
};