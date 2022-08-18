#pragma once

#include "DimensionTagAction.h"
#include "ImagesAction.h"

#include <actions/WidgetAction.h>
#include <actions/StringAction.h>
#include <actions/OptionAction.h>

#include <QTreeView>

class ImageLoaderPlugin;

using namespace hdps::gui;

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
    StringAction& getFilterAction() { return _filterAction;  }
    StringAction& getDatasetNameAction() { return _datasetNameAction; }
    OptionAction& getLoadAsAction() { return _loadAsAction; }
    DimensionTagAction& getDimensionTagAction() { return _dimensionTagAction; }
    ImagesAction& getImagesAction() { return _imagesAction; }

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;     /** Reference to image loader plugin instance */
    StringAction            _filterAction;          /** Filter by image name action */
    StringAction            _datasetNameAction;     /** Produced dataset name action */
    OptionAction            _loadAsAction;          /** Image data layout action */
    DimensionTagAction      _dimensionTagAction;    /** Dimension tag action (in case of multi-page TIFF) */
    ImagesAction            _imagesAction;          /** Images action */
};