#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include <PointData.h>

#include <QDebug>

using namespace hdps;
using namespace hdps::plugin;

Q_PLUGIN_METADATA(IID "nl.BioVault.ImageLoaderPlugin")

ImageLoaderPlugin::ImageLoaderPlugin(const PluginFactory* factory) :
    LoaderPlugin(factory),
    _imageCollectionScanner(*this),
    _imageCollectionsModel(this),
    _imageCollectionsFilterModel(),
    _conversionPickerAction(this, "Conversion")
{
    _imageCollectionsFilterModel.setSourceModel(&_imageCollectionsModel);

    _imageCollectionsModel.selectionModel().setModel(&_imageCollectionsFilterModel);

    _imageCollectionScanner.loadSettings();
    _imageCollectionScanner.scan();

    _conversionPickerAction.initialize("PointDataConversion", { PointType });
}

void ImageLoaderPlugin::loadData()
{
    ImageLoaderDialog dialog(*this);

    dialog.exec();
}

QModelIndex ImageLoaderPlugin::getSelectedImageCollectionIndex() const
{
    const auto selectedRows = _imageCollectionsModel.selectionModel().selectedRows();

    if (selectedRows.isEmpty())
        return QModelIndex();

    return _imageCollectionsFilterModel.mapToSource(selectedRows.first());
}

QIcon ImageLoaderPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return hdps::Application::getIconFont("FontAwesome").getIcon("images", color);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin(this);
}