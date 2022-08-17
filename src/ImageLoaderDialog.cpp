#include "ImageLoaderDialog.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

ImageLoaderDialog::ImageLoaderDialog(ImageLoaderPlugin& imageLoaderPlugin) :
    _imageLoaderPlugin(imageLoaderPlugin),
    _scanAction(this, imageLoaderPlugin),
    _imageCollectionsAction(this, imageLoaderPlugin),
    _imagesAction(this, imageLoaderPlugin),
    _subsamplingAction(this, imageLoaderPlugin),
    _closeAfterLoadingAction(this, "Close after loading", true, true),
    _loadAction(this, "Load")
{
    setWindowIcon(hdps::Application::getIconFont("FontAwesome").getIcon("images"));

    _closeAfterLoadingAction.setToolTip("Close the dialog when loading is complete");
    _closeAfterLoadingAction.setSettingsPrefix(&imageLoaderPlugin, "CloseAfterLoaded");

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &ImageLoaderDialog::updateActions);

    updateActions();

    auto mainLayout = new QVBoxLayout();

    //mainLayout->setContentsMargins(4, 4, 4, 4);
    //mainLayout->setSpacing(0);
    mainLayout->addWidget(_scanAction.createWidget(this));
    mainLayout->addWidget(_imageCollectionsAction.createWidget(this), 2);
    mainLayout->addWidget(_imagesAction.createWidget(this), 1);
    mainLayout->addWidget(_subsamplingAction.createWidget(this));

    auto bottomLayout = new QHBoxLayout();

    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(_closeAfterLoadingAction.createWidget(this));
    bottomLayout->addWidget(_loadAction.createWidget(this));

    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

ImageLoaderDialog::~ImageLoaderDialog() = default;

QSize ImageLoaderDialog::sizeHint() const
{
    return QSize(1024, 768);
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
