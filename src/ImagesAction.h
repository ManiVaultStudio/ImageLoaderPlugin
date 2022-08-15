#pragma once

#include <actions/WidgetAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class ImagesAction : public WidgetAction
{
protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, ImagesAction* imagesAction, const std::int32_t& widgetFlags);
    };

public:

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    ImagesAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin);

public:

    //DirectoryPickerAction& getScanDirectoryAction() { return _scanDirectoryAction; }
    //ToggleAction& getSeparateByDirectoryAction() { return _separateByDirectoryAction; }
    //StringAction& getFilterAction() { return _filterAction; }
    //StringAction& getNameAction() { return _nameAction; }

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;
    //DirectoryPickerAction   _scanDirectoryAction;
    //ToggleAction            _separateByDirectoryAction;
    //StringAction            _filterAction;
    //StringAction            _nameAction;

    friend class Widget;
};