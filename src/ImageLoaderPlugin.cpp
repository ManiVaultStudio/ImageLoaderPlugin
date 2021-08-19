#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"
#include "Set.h"
#include "PointData.h"

#include <QtCore>
#include <QDebug>

#include "PointData.h"

using namespace hdps;

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoaderPlugin")

ImageLoaderPlugin::ImageLoaderPlugin(const PluginFactory* factory) :
    LoaderPlugin(factory),
    _imageCollectionsModel(this),
    _imageCollectionsFilterModel()
{
    _imageCollectionsFilterModel.setSourceModel(&_imageCollectionsModel);

    _imageCollectionsModel.selectionModel().setModel(&_imageCollectionsFilterModel);
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

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin(this);
}

hdps::DataTypes ImageLoaderPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}
