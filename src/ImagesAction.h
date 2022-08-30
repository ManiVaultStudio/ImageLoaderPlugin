#pragma once

#include <actions/WidgetAction.h>
#include <actions/TriggerAction.h>

#include <QTreeView>

class ImageLoaderPlugin;

using namespace hdps::gui;

class ImagesAction : public WidgetAction
{
protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, ImagesAction* imagesAction, const std::int32_t& widgetFlags);

    private:
        void updateTreeView();

    private:
        ImagesAction*   _imagesAction;
        QTreeView       _treeView;
    };

public:

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    ImagesAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;
    TriggerAction           _selectAllAction;
    TriggerAction           _selectNoneAction;
    TriggerAction           _selectInvertAction;

    friend class Widget;
};