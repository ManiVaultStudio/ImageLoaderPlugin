#include "ImageCollectionsAction.h"
#include "ImageLoaderPlugin.h"

#include <QTreeView>

using namespace hdps::gui;

ImageCollectionsAction::ImageCollectionsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setText("Image collections");
}

ImageCollectionsAction::Widget::Widget(QWidget* parent, ImageCollectionsAction* imageCollectionsAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, imageCollectionsAction, widgetFlags)
{
    auto mainLayout = new QGridLayout();

    auto treeView = new QTreeView();

    mainLayout->addWidget(treeView);

    setPopupLayout(mainLayout);

    layout()->setContentsMargins(0, 0, 0, 0);
}
