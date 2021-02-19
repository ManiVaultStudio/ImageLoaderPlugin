#include "CommonSettingsWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_CommonSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

CommonSettingsWidget::CommonSettingsWidget(QWidget* parent) :
    QWidget(parent),
    _imageLoaderPlugin(nullptr),
    _ui(new Ui::CommonSettingsWidget()),
    _scanner()
{
    _ui->setupUi(this);
}

CommonSettingsWidget::~CommonSettingsWidget() = default;

void CommonSettingsWidget::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
    _imageLoaderPlugin = imageLoaderPlugin;

    _scanner.setImageLoaderPlugin(imageLoaderPlugin);

    _ui->separateByDirectoryCheckBox->setChecked(_scanner.getSeparateByDirectory());

    auto& imageCollectionsModel             = _imageLoaderPlugin->getImageCollectionsModel();
    auto& imageCollectionsSelectionModel    = imageCollectionsModel.selectionModel();
    auto& filterModel                       = _imageLoaderPlugin->getImageCollectionsFilterModel();

    _ui->imageCollectionsTreeView->setModel(&_imageLoaderPlugin->getImageCollectionsFilterModel());
    _ui->imageCollectionsTreeView->setSelectionModel(&imageCollectionsModel.selectionModel());

    const auto selectedImageCollection = [&]() {
        const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

        if (selectedRows.isEmpty())
            return QModelIndex();

        return filterModel.mapToSource(selectedRows.first());
    };

    // Column visibility
    for (int column = ult(ImageCollection::Image::Column::Start); column <= ult(ImageCollection::Image::Column::End); column++)
        _ui->imageCollectionsTreeView->header()->hideSection(column);

    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::DatasetName));
    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::ImageType));
    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::ImageFormat));
    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::ToGrayscale));
    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::NoImages));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::NoSelectedImages));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::DimensionTag));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::IsMultiPage));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SourceSize));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::TargetSize));
    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::TargetWidth));
    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::TargetHeight));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::Type));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SubsamplingEnabled));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SubsamplingRatio));
    _ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SubsamplingFilter));
    //_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::Directory));

    _ui->imagesTreeView->header()->setHidden(true);

    _ui->selectPercentageDoubleSpinBox->setValue(_imageLoaderPlugin->getSetting("Miscellaneous/SelectPercentage", 50.0).toDouble());

    connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString text) {
        _scanner.setDirectory(text);
    });

    connect(_ui->directoryPushButton, &QPushButton::clicked, [this]() {
        const auto initialDirectory = _scanner.getDirectory();
        const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

        if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
            _scanner.setDirectory(pickedDirectory);
        }
    });

    connect(&_scanner, &ImageCollectionScanner::directoryChanged, [this](const QString& directory) {
        _ui->directoryLineEdit->blockSignals(true);
        _ui->directoryLineEdit->setText(directory);
        _ui->directoryLineEdit->blockSignals(false);
    });

    connect(_ui->separateByDirectoryCheckBox, &QCheckBox::stateChanged, [this](int state) {
        _scanner.setSeparateByDirectory(state == Qt::Checked);
    });

    connect(&_scanner, &ImageCollectionScanner::separateByDirectoryChanged, [this](const bool& separateByDirectory) {
        _ui->separateByDirectoryCheckBox->blockSignals(true);
        _ui->separateByDirectoryCheckBox->setChecked(separateByDirectory);
        _ui->separateByDirectoryCheckBox->blockSignals(false);
    });

    connect(_ui->searchFilterLineEdit, &QLineEdit::textChanged, [this](QString text) {
        _scanner.setFilenameFilter(text);
    });

    connect(&_scanner, &ImageCollectionScanner::filenameFilterChanged, [this](const QString& filenameFilter) {
        //_ui->searchFilterLineEdit->setFocus();
        _ui->searchFilterLineEdit->blockSignals(true);
        _ui->searchFilterLineEdit->setText(filenameFilter);
        _ui->searchFilterLineEdit->blockSignals(false);

        _imageLoaderPlugin->getImageCollectionsFilterModel().setFilter(filenameFilter);
    });

    connect(_ui->loadAsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [&, selectedImageCollection](int currentIndex) {
        const auto index = selectedImageCollection();

        if (index != QModelIndex())
            imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::Type)), currentIndex);
    });

    connect(&imageCollectionsModel, &ImageCollectionsModel::rowsInserted, [&, this]() {
        if (imageCollectionsModel.rowCount(QModelIndex()) > 0) {
            _ui->imageCollectionsTreeView->setEnabled(true);
            _ui->imageCollectionsLabel->setEnabled(true);

            _ui->imageCollectionsTreeView->header()->show();

            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::DatasetName));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::ImageType));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::ImageFormat));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::ToGrayscale));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::NoImages));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::NoSelectedImages));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::SourceSize));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::TargetSize));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::TargetWidth));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::TargetHeight));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::NoPoints));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::NoDimensions));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::Memory));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::Directory));
            _ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::Type));

            _ui->searchFilterLineEdit->setEnabled(true);
        }
    });

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
        _ui->selectPercentageDoubleSpinBox->setEnabled(false);
        _ui->selectPercentagePushButton->setEnabled(false);
    });

    connect(_ui->selectAllPushButton, &QPushButton::clicked, [&, selectedImageCollection]() {
        const auto index = selectedImageCollection();

        if (index != QModelIndex())
            imageCollectionsModel.selectAll(index);
    });

    connect(_ui->selectNonePushButton, &QPushButton::clicked, [&, selectedImageCollection]() {
        const auto index = selectedImageCollection();

        if (index != QModelIndex())
            imageCollectionsModel.selectNone(index);
    });

    connect(_ui->invertSelectionPushButton, &QPushButton::clicked, [&, selectedImageCollection]() {
        const auto index = selectedImageCollection();

        if (index != QModelIndex())
            imageCollectionsModel.invertSelection(index);
    });

    connect(_ui->selectPercentageDoubleSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), [this](double value) {
        _imageLoaderPlugin->setSetting("Miscellaneous/SelectPercentage", value);
    });

    connect(_ui->selectPercentagePushButton, &QPushButton::clicked, [&, selectedImageCollection]() {
        const auto index = selectedImageCollection();

        if (index != QModelIndex())
            imageCollectionsModel.selectPercentage(index, 0.01f * _ui->selectPercentageDoubleSpinBox->value());
    });

    auto updateSelectionButtons = [&](const QModelIndex& index) {
        const auto noImages = imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::NoImages)), Qt::EditRole).toInt();
        const auto noSelectedImages = imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages)), Qt::EditRole).toInt();

        _ui->selectAllPushButton->setEnabled(noSelectedImages != noImages);
        _ui->selectNonePushButton->setEnabled(noSelectedImages > 0);
        _ui->invertSelectionPushButton->setEnabled(true);
        _ui->selectPercentageDoubleSpinBox->setEnabled(noImages > 1);
        _ui->selectPercentagePushButton->setEnabled(noImages > 1);
    };

    const auto updateImagesHeader = [&, selectedImageCollection]() {
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

        const auto index = selectedImageCollection();

        if (index != QModelIndex()) {
            const auto type = imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::EditRole).toInt();
            _ui->imagesTreeView->header()->setSectionHidden(ult(ImageCollection::Image::Column::DimensionName), type == ult(ImageData::Type::Sequence));
        }

        _ui->imagesTreeView->header()->hideSection(ult(ImageCollection::Image::Column::FileName));
    };

    connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, [&, selectedImageCollection, updateImagesHeader, updateSelectionButtons](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles) {
        const auto index = selectedImageCollection();

        if (index != QModelIndex() && topLeft.row() == index.row()) {
            for (int column = topLeft.column(); column <= bottomRight.column(); column++) {
                switch (static_cast<ImageCollection::Column>(column))
                {
                    case ImageCollection::Column::Type:
                    {
                        updateImagesHeader();
                        break;
                    }

                    case ImageCollection::Column::NoSelectedImages:
                    {
                        updateSelectionButtons(index);
                        break;
                    }

                    default:
                        break;
                }
            }
        }
    });

    connect(_ui->imageCollectionsTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, [&, selectedImageCollection, updateImagesHeader, updateSelectionButtons](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto index = selectedImageCollection();

        if (index != QModelIndex()) {
            _ui->loadAsLabel->setEnabled(true);
            _ui->loadAsComboBox->setEnabled(true);
            _ui->imagesLabel->setEnabled(true);
            _ui->imagesTreeView->setEnabled(true);
            _ui->imagesTreeView->setModel(&imageCollectionsModel);
            _ui->imagesTreeView->setRootIndex(index);
            _ui->imagesTreeView->header()->setHidden(false);
            
            const auto dimensionTagEnabled = index.siblingAtColumn(ult(ImageCollection::Column::IsMultiPage)).data().toBool();

            _ui->dimensionTagLabel->setEnabled(dimensionTagEnabled);
            _ui->dimensionTagComboBox->setEnabled(dimensionTagEnabled);

            _ui->loadAsComboBox->setCurrentIndex(imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::EditRole).toInt());

            imageCollectionsModel.guessDimensionNames(index);

            updateImagesHeader();
            updateSelectionButtons(index);
        }
    });

    _scanner.loadSettings();
    _scanner.scan();
}