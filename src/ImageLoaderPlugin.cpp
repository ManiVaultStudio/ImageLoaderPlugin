#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include <PointData/PointData.h>

#include <QDebug>

using namespace mv;
using namespace mv::plugin;

Q_PLUGIN_METADATA(IID "nl.BioVault.ImageLoaderPlugin")

ImageLoaderPlugin::ImageLoaderPlugin(const PluginFactory* factory) :
    LoaderPlugin(factory),
    _imageCollectionScanner(*this),
    _imageCollectionsModel(this),
    _conversionPickerAction(this, *this)
{
    _imageCollectionsFilterModel.setSourceModel(&_imageCollectionsModel);

    _imageCollectionsModel.selectionModel().setModel(&_imageCollectionsFilterModel);

    _imageCollectionScanner.loadSettings();
    _imageCollectionScanner.scan();

    _conversionPickerAction.initialize("PointDataConversion", { PointType });
}

ImageLoaderPlugin::~ImageLoaderPlugin()
{
}

void ImageLoaderPlugin::loadData()
{
    ImageLoaderDialog dialog(*this);

    dialog.open();

    QEventLoop eventLoop;

    QObject::connect(&dialog, &QDialog::finished, &eventLoop, &QEventLoop::quit);

    eventLoop.exec();
}

QModelIndexList ImageLoaderPlugin::getSelectedRows() const
{
    QModelIndexList selectedImageCollectionIndices;

    for (const auto& selectedRow : _imageCollectionsModel.selectionModel().selectedRows())
        selectedImageCollectionIndices << _imageCollectionsFilterModel.mapToSource(selectedRow);

    return selectedImageCollectionIndices;
}

ImageLoaderPluginFactory::ImageLoaderPluginFactory()
{
    setIconByName("images");
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin(this);
}
