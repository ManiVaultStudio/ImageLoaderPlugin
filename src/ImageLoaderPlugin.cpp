#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include <PointData/PointData.h>

#include <widgets/MarkdownDialog.h>

#include <QDebug>

using namespace mv;
using namespace mv::plugin;

Q_PLUGIN_METADATA(IID "studio.manivault.ImageLoaderPlugin")

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

    connect(&getPluginMetadata().getTriggerHelpAction(), &TriggerAction::triggered, this, [this]() -> void {
        if (!getReadmeMarkdownUrl().isValid() || _helpMarkdownDialog.get())
            return;

        _helpMarkdownDialog = new util::MarkdownDialog(getReadmeMarkdownUrl());

        _helpMarkdownDialog->setWindowTitle(QString("%1").arg(getKind()));
        _helpMarkdownDialog->setAttribute(Qt::WA_DeleteOnClose);
        _helpMarkdownDialog->setWindowModality(Qt::NonModal);
        _helpMarkdownDialog->show();
        });
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin(this);
}

QUrl ImageLoaderPluginFactory::getReadmeMarkdownUrl() const
{
    return QUrl("https://raw.githubusercontent.com/ManiVaultStudio/ImageLoaderPlugin/master/README.md");
}

QUrl ImageLoaderPluginFactory::getRepositoryUrl() const
{
    return QUrl("https://github.com/ManiVaultStudio/ImageLoaderPlugin");
}
