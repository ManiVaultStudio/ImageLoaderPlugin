#pragma once

#include "DataLayoutAction.h"
#include "DimensionTagAction.h"
#include "ImagesAction.h"

#include <actions/WidgetAction.h>
#include <actions/StringAction.h>
#include <actions/OptionAction.h>
#include <actions/ToggleAction.h>
#include <actions/DecimalAction.h>

#include <QTreeView>

class ImageLoaderPlugin;

using namespace hdps::gui;

class ImageCollectionsAction : public WidgetAction
{
public:

    /** Subsampling scale factors */
    enum ScaleFactor {
        Perc5,      /** Scale by 5% */
        Perc10,     /** Scale by 10% */
        Perc25,     /** Scale by 25% */
        Perc50,     /** Scale by 50% */
        Perc75      /** Scale by 75% */
    };

    static const QMap<ScaleFactor, TriggersAction::Trigger> triggers;     /** Maps scale enum to trigger */
    static const QMap<ScaleFactor, float> scaleFactors;                   /** Maps scale enum to scale factor */

protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, ImageCollectionsAction* imageCollectionsAction, const std::int32_t& widgetFlags);

    private:
        void updateTreeView();

    private:
        ImageCollectionsAction* _imageCollectionsAction;
        QTreeView               _treeView;
    };

public:

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    ImageCollectionsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin);

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;                 /** Reference to image loader plugin instance */
    StringAction            _filterAction;                      /** Filter by image name action */
    StringAction            _datasetNameAction;                 /** Produced dataset name action */
    OptionAction            _loadAsAction;                      /** Image data layout action */
    DataLayoutAction        _dataLayoutAction;                  /** Data layout action (stack/sequence) */
    DimensionTagAction      _dimensionTagAction;                /** Dimension tag action (in case of multi-page TIFF) */
    ToggleAction            _subsamplingEnabledAction;          /** Subsampling enabled action */
    DecimalAction           _subsamplingRatioAction;            /** Subsampling ratio action */
    TriggersAction          _subsamplingDefaultRatiosAction;    /** Default subsampling ratios action */
    OptionAction            _subsamplingFilterTypeAction;       /** Subsampling filter type action */
    ImagesAction            _imagesAction;                      /** Images action */

    friend class Widget;
};