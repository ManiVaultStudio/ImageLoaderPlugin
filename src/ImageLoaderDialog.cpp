#include "ImageLoaderDialog.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

ImageLoaderDialog::ImageLoaderDialog(ImageLoaderPlugin& imageLoaderPlugin) :
    _imageLoaderPlugin(imageLoaderPlugin),
    _commonSettingsAction(this, imageLoaderPlugin),
    _subsamplingAction(this, imageLoaderPlugin),
    _closeAfterLoadingAction(this, "Close after loading", true, true),
    _loadAction(this, "Load")
{
    setWindowIcon(hdps::Application::getIconFont("FontAwesome").getIcon("images"));

    _closeAfterLoadingAction.setToolTip("Close the dialog when loading is complete");
    _closeAfterLoadingAction.setChecked(_imageLoaderPlugin.getSetting("GUI/CloseAfterLoaded", true).toBool());

    connect(&_closeAfterLoadingAction, &ToggleAction::toggled, [this]() {
        _imageLoaderPlugin.setSetting("GUI/CloseAfterLoaded", _closeAfterLoadingAction.isChecked());
    });

    auto mainLayout = new QVBoxLayout();

    //mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->addWidget(_commonSettingsAction.createWidget(this), 1);
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
    return QSize(800, 600);
}

/*
void ImageLoaderDialog::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
    _ui->closeAfterLoadedCheckBox->setChecked();

    auto& imageCollectionsModel             = _imageLoaderPlugin->getImageCollectionsModel();
    auto& imageCollectionsSelectionModel    = imageCollectionsModel.selectionModel();
    auto& filterModel                       = _imageLoaderPlugin->getImageCollectionsFilterModel();

    const auto selectedRow = [&]() {
        const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

        if (selectedRows.isEmpty())
            return QModelIndex();

        return filterModel.mapToSource(selectedRows.first());
    };

    const auto updateLoadButton = [&, selectedRow]() {
        const auto index = selectedRow();

        _ui->loadPushButton->setEnabled(false);

        if (index != QModelIndex()) {
            const auto imageCollectionType  = imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::DisplayRole).toString();
            const auto noSelectedImages     = imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages)), Qt::EditRole).toInt();

            _ui->loadPushButton->setEnabled(noSelectedImages > 0);
            _ui->loadPushButton->setText(QString("Load %1").arg(imageCollectionType));
        }
    };

    QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [&, selectedRow, updateLoadButton](const QItemSelection& selected, const QItemSelection& deselected) {
        updateLoadButton();
    });

    QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, [&, selectedRow, updateLoadButton](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles) {
        updateLoadButton();
    });

    QObject::

    QObject::connect(_ui->loadPushButton, &QPushButton::clicked, [&, selectedRow]() {
        const auto index = selectedRow();

        if (index != QModelIndex()) {
            const auto loaded = imageCollectionsModel.loadImageCollection(_imageLoaderPlugin, index);

            if (loaded && _imageLoaderPlugin->getSetting("GUI/CloseAfterLoaded", true).toBool())
                this->close();
        }
    });
}
*/