#include "ImageLoaderDialog.h"
#include "ImageLoaderPlugin.h"
#include "Application.h"

#include <QDebug>

using namespace hdps;

ImageLoaderDialog::ImageLoaderDialog(ImageLoaderPlugin& imageLoaderPlugin) :
    _imageLoaderPlugin(imageLoaderPlugin),
    _scanAction(this, imageLoaderPlugin),
    _imageCollectionsAction(this, imageLoaderPlugin),
    _subsamplingAction(this, imageLoaderPlugin),
    _closeAfterLoadingAction(this, "Close after loading", true, true),
    _loadAction(this, "Load"),
    _cancelAction(this, "Cancel")
{
    setWindowTitle("Load high-dimensional image data");
    setWindowIcon(hdps::Application::getIconFont("FontAwesome").getIcon("images"));

    _closeAfterLoadingAction.setToolTip("Close the dialog when loading is complete");
    _closeAfterLoadingAction.setSettingsPrefix(&imageLoaderPlugin, "CloseAfterLoaded");

    _loadAction.setToolTip("Load the selected image collections");
    _cancelAction.setToolTip("Close the image loader");

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &ImageLoaderDialog::updateActions);
    connect(&_loadAction, &TriggerAction::triggered, this, &ImageLoaderDialog::loadImageCollections);
    connect(&_cancelAction, &TriggerAction::triggered, this, &ImageLoaderDialog::reject);

    updateActions();

    auto mainLayout = new QVBoxLayout();

    //mainLayout->setContentsMargins(4, 4, 4, 4);
    //mainLayout->setSpacing(0);
    mainLayout->addWidget(_scanAction.createWidget(this));
    mainLayout->addWidget(_imageCollectionsAction.createWidget(this), 2);
    mainLayout->addWidget(_subsamplingAction.createWidget(this));

    auto bottomLayout = new QHBoxLayout();

    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(_closeAfterLoadingAction.createWidget(this));
    bottomLayout->addWidget(_loadAction.createWidget(this));
    bottomLayout->addWidget(_cancelAction.createWidget(this));

    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

ImageLoaderDialog::~ImageLoaderDialog() = default;

QSize ImageLoaderDialog::sizeHint() const
{
    return QSize(1024, 1024);
}

void ImageLoaderDialog::updateActions()
{
    const auto selectedRows = _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows();
    const auto hasSelection = !selectedRows.isEmpty();

    _loadAction.setEnabled(hasSelection);

    if (selectedRows.isEmpty()) {
        _loadAction.setText("Load");
        _loadAction.setToolTip("Load image datasets into HDPS");
    }
    else {
        _loadAction.setText(QString("Load %1").arg(QString::number(selectedRows.count())));
        _loadAction.setToolTip(QString("Load %1 image datasets into HDPS").arg(QString::number(selectedRows.count())));
    }
    
    _closeAfterLoadingAction.setEnabled(hasSelection);
}

void ImageLoaderDialog::loadImageCollections()
{
    const auto selectedRows = _imageLoaderPlugin.getSelectedRows();
    const auto hasSelection = !selectedRows.isEmpty();

    for (const auto& selectedImageCollectionIndex : _imageLoaderPlugin.getSelectedRows()) {
        const auto imageCollection = reinterpret_cast<ImageCollection*>(selectedImageCollectionIndex.internalPointer());

        imageCollection->load(&_imageLoaderPlugin);
    }

    if (_closeAfterLoadingAction.isChecked())
        accept();
}
