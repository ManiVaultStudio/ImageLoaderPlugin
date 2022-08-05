#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include <Set.h>
#include <PointData.h>
#include <Application.h>

#include <QtCore>
#include <QDebug>

using namespace hdps;

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoaderPlugin")

ImageLoaderPlugin::ImageLoaderPlugin(const PluginFactory* factory) :
    LoaderPlugin(factory),
    _imageCollectionsModel(this),
    _imageCollectionsFilterModel(),
    _pluginTriggerPickerAction(this)
{
    _imageCollectionsFilterModel.setSourceModel(&_imageCollectionsModel);

    _imageCollectionsModel.selectionModel().setModel(&_imageCollectionsFilterModel);

    _pluginTriggerPickerAction.setEnabled(false);
    _pluginTriggerPickerAction.initialize("PointDataConversion", hdps::DataTypes({ PointType }));
}

ImageLoaderPlugin::~ImageLoaderPlugin()
{
}

PluginTriggerPickerAction& ImageLoaderPlugin::getPluginTriggerPickerAction()
{
    return _pluginTriggerPickerAction;
}

void ImageLoaderPlugin::init()
{
}

void ImageLoaderPlugin::loadData()
{
    ImageLoaderDialog dialog;

    dialog.initialize(this);

    dialog.exec();
}

QIcon ImageLoaderPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return hdps::Application::getIconFont("FontAwesome").getIcon("images", color);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin(this);
}