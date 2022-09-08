#include "SubsamplingAction.h"
#include "ImageLoaderPlugin.h"

#include <QGridLayout>
#include <QHBoxLayout>

using namespace hdps::gui;

SubsamplingAction::SubsamplingAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _typeAction(this, imageLoaderPlugin),
    _ratioAction(this, imageLoaderPlugin),
    _filterTypeAction(this, imageLoaderPlugin),
    _levelsAction(this, imageLoaderPlugin)
{
    setText("Subsampling");
    setCheckable(true);
}

SubsamplingAction::Widget::Widget(QWidget* parent, SubsamplingAction* subsamplingAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, subsamplingAction, widgetFlags)
{
}
