#pragma once

#include "DatasetNameAction.h"
#include "DataLayoutAction.h"
#include "DimensionTagAction.h"
#include "SubsamplingAction.h"
#include "AddCoordinatesPointsAction.h"
#include "MirrorAction.h"
#include "ImagesAction.h"

#include <actions/WidgetAction.h>
#include <actions/StringAction.h>
#include <actions/OptionAction.h>
#include <actions/DecimalAction.h>

#include <QTreeView>

class ImageLoaderPlugin;

using namespace mv::gui;

class ImageCollectionsAction : public WidgetAction
{
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

public:

    StringAction& getFilterAction() { return _filterAction; }
    DatasetNameAction& getDatasetNameAction() { return _datasetNameAction; }
    DataLayoutAction& getDataLayoutAction() { return _dataLayoutAction; }
    DimensionTagAction& getDimensionTagAction() { return _dimensionTagAction; }
    SubsamplingAction& getSubsamplingAction() { return _subsamplingAction; }
    AddCoordinatesPointsAction& getAddCoordinatesPointsAction() { return _addCoordinatesPointsAction; }
    MirrorAction& getMirrorAction() { return _mirrorAction; }
    ImagesAction& getImagesAction() { return _imagesAction; }

protected:
    ImageLoaderPlugin&          _imageLoaderPlugin;             /** Reference to image loader plugin instance */
    StringAction                _filterAction;                  /** Filter by image name action */
    DatasetNameAction           _datasetNameAction;             /** Produced dataset name action */
    DataLayoutAction            _dataLayoutAction;              /** Data layout action (stack/sequence) */
    DimensionTagAction          _dimensionTagAction;            /** Dimension tag action (in case of multi-page TIFF) */
    SubsamplingAction           _subsamplingAction;             /** Subsampling action */
    AddCoordinatesPointsAction  _addCoordinatesPointsAction;    /** Add 2D coordinates points action */
    MirrorAction                _mirrorAction;                  /** Mirror action */
    ImagesAction                _imagesAction;                  /** Images action */

};