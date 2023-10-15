#include "SubsamplingAction.h"
#include "ImageLoaderPlugin.h"

#include <QGridLayout>
#include <QHBoxLayout>

using namespace mv::gui;

SubsamplingAction::SubsamplingAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent, "Subsampling"),
    _imageLoaderPlugin(imageLoaderPlugin),
    _typeAction(this, imageLoaderPlugin),
    _ratioAction(this, imageLoaderPlugin),
    _filterTypeAction(this, imageLoaderPlugin),
    _levelsAction(this, imageLoaderPlugin)
{
    setCheckable(true);
}

SubsamplingAction::Widget::Widget(QWidget* parent, SubsamplingAction* subsamplingAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, subsamplingAction, widgetFlags)
{
}
