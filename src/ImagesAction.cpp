#include "ImagesAction.h"
#include "ImageLoaderPlugin.h"

#include <QTreeView>

using namespace hdps::gui;

ImagesAction::ImagesAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setText("Images");
}

ImagesAction::Widget::Widget(QWidget* parent, ImagesAction* imagesAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, imagesAction, widgetFlags)
{
    auto mainLayout = new QGridLayout();

    auto treeView = new QTreeView();

    mainLayout->addWidget(treeView, 0, 1);
    mainLayout->addWidget(treeView, 0, 1);

    setPopupLayout(mainLayout);

    layout()->setContentsMargins(0, 0, 0, 0);
}
