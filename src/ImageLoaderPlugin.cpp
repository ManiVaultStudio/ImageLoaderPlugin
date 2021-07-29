#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"
#include "Set.h"
#include "PointData.h"

#include <QtCore>
#include <QDebug>

#include "PointData.h"

using namespace hdps;

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoaderPlugin")

ImageLoaderPlugin::ImageLoaderPlugin() :
    LoaderPlugin("Image Loader"),
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

hdps::DataTypes ImageLoaderPlugin::supportedDataTypes() const
{
	DataTypes supportedTypes;
	supportedTypes.append(PointType);
	return supportedTypes;
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}