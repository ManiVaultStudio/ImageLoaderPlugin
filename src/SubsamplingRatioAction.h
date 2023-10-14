#pragma once

#include "SubsamplingTypeAction.h"
#include "LevelsAction.h"

#include <actions/WidgetAction.h>
#include <actions/DecimalAction.h>
#include <actions/TriggersAction.h>
#include <actions/OptionAction.h>
#include <actions/ToggleAction.h>

class ImageLoaderPlugin;

using namespace mv::gui;

class SubsamplingRatioAction : public WidgetAction
{
public:

    /** Subsampling ratio factors */
    enum Ratio {
        Perc5,      /** Scale by 5% */
        Perc10,     /** Scale by 10% */
        Perc25,     /** Scale by 25% */
        Perc50,     /** Scale by 50% */
        Perc75      /** Scale by 75% */
    };

    static const QMap<Ratio, TriggersAction::Trigger> triggers;     /** Maps ratio enum to trigger */
    static const QMap<Ratio, float> defaultRatios;                  /** Maps ratio enum to ratio value */

protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, SubsamplingRatioAction* subsamplingRatioAction, const std::int32_t& widgetFlags);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    SubsamplingRatioAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void updateRows();
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles = QList<int>());
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;         /** Reference to parent image loader plugin */
    DecimalAction           _ratioAction;               /** Subsampling ratio action */
    TriggersAction          _defaultRatiosAction;       /** Default subsampling ratios action */

};