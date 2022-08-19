#include "CommonSettingsWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_CommonSettingsWidget.h"

#include <PointData.h>

#include <QDebug>
#include <QFileDialog>
#include <QDir>

CommonSettingsWidget::CommonSettingsWidget(QWidget* parent) :
    QWidget(parent),
    _imageLoaderPlugin(nullptr),
    _ui(new Ui::CommonSettingsWidget())
{
}

CommonSettingsWidget::~CommonSettingsWidget() = default;

void CommonSettingsWidget::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
    _imageLoaderPlugin = imageLoaderPlugin;

    auto& imageCollectionsModel             = _imageLoaderPlugin->getImageCollectionsModel();
    auto& imageCollectionsSelectionModel    = imageCollectionsModel.selectionModel();
    auto& filterModel                       = _imageLoaderPlugin->getImageCollectionsFilterModel();

    _ui->imageCollectionsTreeView->setModel(&_imageLoaderPlugin->getImageCollectionsFilterModel());
    _ui->imageCollectionsTreeView->setSelectionModel(&imageCollectionsModel.selectionModel());

    const auto selectedImageCollectionIndex = [&]() {
        const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

        if (selectedRows.isEmpty())
            return QModelIndex();

        return filterModel.mapToSource(selectedRows.first());
    };

    _ui->imagesTreeView->header()->setHidden(true);

    _ui->selectPercentageDoubleSpinBox->setValue(_imageLoaderPlugin->getSetting("Miscellaneous/SelectPercentage", 50.0).toDouble());

    connect(&imageCollectionsModel, &ImageCollectionsModel::modelReset, [&, this]() {
        _ui->imageCollectionsTreeView->setEnabled(false);
        _ui->imageCollectionsLabel->setEnabled(false);
        _ui->imageCollectionsTreeView->header()->hide();
        _ui->imagesTreeView->setModel(nullptr);
        _ui->searchFilterLineEdit->setEnabled(false);
        _ui->loadAsLabel->setEnabled(false);
        _ui->loadAsComboBox->setEnabled(false);
        _ui->imagesLabel->setEnabled(false);
        _ui->imagesTreeView->setEnabled(false);
        _ui->selectAllPushButton->setEnabled(false);
        _ui->selectNonePushButton->setEnabled(false);
        _ui->invertSelectionPushButton->setEnabled(false);
    });

    const auto updateImagesHeader = [&, selectedImageCollectionIndex]() {
        for (int column = ult(ImageCollection::Column::Start); column <= ult(ImageCollection::Column::End); column++)
            _ui->imagesTreeView->header()->hideSection(column);

        _ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::ShouldLoad), QHeaderView::Fixed);
        _ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::FileName), QHeaderView::ResizeToContents);
        _ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::DimensionName), QHeaderView::Interactive);
        _ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::FileName), QHeaderView::Interactive);

        _ui->imagesTreeView->header()->setMinimumSectionSize(20);
        _ui->imagesTreeView->header()->resizeSection(ult(ImageCollection::Image::Column::ShouldLoad), 20);

        _ui->imagesTreeView->resizeColumnToContents(ult(ImageCollection::Image::Column::FileName));
        _ui->imagesTreeView->resizeColumnToContents(ult(ImageCollection::Image::Column::DimensionName));

        const auto index = selectedImageCollectionIndex();

        if (index != QModelIndex()) {
            const auto type = imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::EditRole).toInt();
            _ui->imagesTreeView->header()->setSectionHidden(ult(ImageCollection::Image::Column::DimensionName), type == ult(ImageData::Type::Sequence));
        }

        _ui->imagesTreeView->header()->hideSection(ult(ImageCollection::Image::Column::FileName));
    };

    /*
    connect(_ui->imageCollectionsTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, [&, imageLoaderPlugin, selectedImageCollectionIndex, updateImagesHeader, updateSelectionButtons, updateTagUI](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto index = selectedImageCollectionIndex();

        imageLoaderPlugin->getConversionPickerAction().setEnabled(index != QModelIndex());

        if (index != QModelIndex()) {
            _ui->loadAsLabel->setEnabled(true);
            _ui->loadAsComboBox->setEnabled(true);
            _ui->imagesLabel->setEnabled(true);
            _ui->imagesTreeView->setEnabled(true);
            _ui->imagesTreeView->setModel(&imageCollectionsModel);
            _ui->imagesTreeView->setRootIndex(index);
            _ui->imagesTreeView->header()->setHidden(false);

            _ui->loadAsComboBox->setCurrentIndex(imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::EditRole).toInt());

            imageLoaderPlugin->getConversionPickerAction().reset();
            imageLoaderPlugin->getConversionPickerAction().setCurrentPluginTriggerAction(imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Conversion)), Qt::EditRole).toString());

            imageCollectionsModel.guessDimensionNames(index);

            updateImagesHeader();
            updateSelectionButtons(index);
            updateTagUI();
        }
    });
    */
}