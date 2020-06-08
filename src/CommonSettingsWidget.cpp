#include "CommonSettingsWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_CommonSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

CommonSettingsWidget::CommonSettingsWidget(QWidget* parent) :
	QWidget(parent),
	_imageLoaderPlugin(nullptr),
	_ui{ std::make_unique<Ui::CommonSettingsWidget>() },
	_scanner(),
	_loader(nullptr, ImageData::Type::Sequence)
{
	_ui->setupUi(this);
}

CommonSettingsWidget::~CommonSettingsWidget() = default;

void CommonSettingsWidget::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_scanner.setImageLoaderPlugin(imageLoaderPlugin);

	_ui->separateByDirectoryCheckBox->setChecked(_scanner.separateByDirectory());

	auto& imageCollectionsModel	= _imageLoaderPlugin->imageCollectionsModel();
	auto& imagesModel			= _imageLoaderPlugin->imagesModel();

	_ui->imageCollectionsTreeView->setModel(&imageCollectionsModel);
	_ui->imageCollectionsTreeView->setSelectionModel(&imageCollectionsModel.selectionModel());

	// Column visibility
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::DatasetName));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::NoImages));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::NoSelectedImages));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SourceSize));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::TargetSize));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::TargetWidth));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::TargetHeight));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::Type));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SubsamplingEnabled));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SubsamplingRatio));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::SubsamplingFilter));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::Grayscale));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollection::Column::Directory));

	_ui->imagesTreeView->setModel(&imageCollectionsModel);
	//_ui->imagesTreeView->setSelectionModel(&imageCollectionsModel.selectionModel());
	_ui->imagesTreeView->header()->setHidden(true);
	
	// Column visibility
	//_ui->imagesTreeView->header()->hideSection(ult(ImagesModel::Column::ShouldLoad));
	//_ui->imagesTreeView->header()->hideSection(ult(ImagesModel::Column::Name));
	//_ui->imagesTreeView->header()->hideSection(ult(ImagesModel::Column::DimensionName));
	//_ui->imagesTreeView->header()->hideSection(ult(ImagesModel::Column::FilePath));
	
	// Column resize mode
	_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImagesModel::Column::ShouldLoad), QHeaderView::Fixed);
	_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImagesModel::Column::FileName), QHeaderView::Interactive);
	_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImagesModel::Column::DimensionName), QHeaderView::Interactive);
	_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImagesModel::Column::FileName), QHeaderView::Interactive);

	// Column size
	_ui->imagesTreeView->header()->setMinimumSectionSize(20);
	_ui->imagesTreeView->header()->resizeSection(ult(ImagesModel::Column::ShouldLoad), 20);
	_ui->imagesTreeView->header()->resizeSection(ult(ImagesModel::Column::FileName), 200);
	_ui->imagesTreeView->header()->resizeSection(ult(ImagesModel::Column::DimensionName), 200);
	_ui->imagesTreeView->header()->resizeSection(ult(ImagesModel::Column::FilePath), 200);

	QObject::connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString directory) {
		_scanner.setDirectory(directory);
	});

	QObject::connect(_ui->directoryPushButton, &QPushButton::clicked, [this]() {
		const auto initialDirectory = _scanner.directory();
		const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

		if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
			_scanner.setDirectory(pickedDirectory);
		}
	});

	QObject::connect(&_scanner, &ImageCollectionScanner::directoryChanged, [this](const QString& directory) {
		_ui->directoryLineEdit->blockSignals(true);
		_ui->directoryLineEdit->setText(directory);
		_ui->directoryLineEdit->blockSignals(false);
	});

	QObject::connect(_ui->separateByDirectoryCheckBox, &QCheckBox::stateChanged, [this](int state) {
		_scanner.setSeparateByDirectory(state == Qt::Checked);
	});

	QObject::connect(&_scanner, &ImageCollectionScanner::separateByDirectoryChanged, [this](const bool& separateByDirectory) {
		_ui->separateByDirectoryCheckBox->blockSignals(true);
		_ui->separateByDirectoryCheckBox->setChecked(separateByDirectory);
		_ui->separateByDirectoryCheckBox->blockSignals(false);
	});

	QObject::connect(_ui->loadAsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &imageCollectionsModel](int currentIndex) {
		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty()) {
			imageCollectionsModel.setData(selectedRows.first().siblingAtColumn(ult(ImageCollection::Column::Type)), currentIndex);
		}
	});

	/*
	auto selectImageCollection = [this, &imageCollectionsModel, &imagesModel](const QModelIndex& index) {
		imagesModel.setImageCollection(const_cast<ImageCollection*>(imageCollectionsModel.imageCollection(index.row())));

		const auto imageCollectionType = imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::EditRole).toInt();

		_ui->loadAsComboBox->setCurrentIndex(imageCollectionType);
		_ui->imagesTreeView->setColumnHidden(ult(ImagesModel::Column::DimensionName), imageCollectionType != ImageData::Type::Stack);
	};
	*/

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::rowsInserted, [this]() {
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::DatasetName));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::ImageType));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::NoImages));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::NoSelectedImages));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::SourceSize));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::TargetSize));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::TargetWidth));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::TargetHeight));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::Directory));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::Type));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollection::Column::ToGrayscale));
	});
	/*
	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::modelReset, [this, &imageCollectionsModel, &imagesModel, selectImageCollection]() {
		imagesModel.setImageCollection(nullptr);
	});

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, [this, &imageCollectionsModel, &imagesModel, selectImageCollection](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles) {
		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty() && topLeft.row() == selectedRows.first().row() && topLeft.column() == ult(ImageCollection::Column::Type)) {
			selectImageCollection(selectedRows.first());
		}
	});
	*/

	QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [this, &imageCollectionsModel, &imagesModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty()) {
			_ui->imagesLabel->setEnabled(true);
			_ui->imagesTreeView->setEnabled(true);
			_ui->imagesTreeView->header()->setHidden(false);

			_ui->imagesTreeView->setRootIndex(selectedRows.first());
		}
			
	});

	/*
	QObject::connect(&imagesModel, &ImagesModel::rowsInserted, [this]() {
		_ui->imagesLabel->setEnabled(true);
		_ui->imagesTreeView->setEnabled(true);
		_ui->imagesTreeView->header()->setHidden(false);
	});
	*/

	QObject::connect(&imagesModel, &ImageCollectionsModel::modelReset, [this]() {
		_ui->imagesLabel->setEnabled(false);
		_ui->imagesTreeView->setEnabled(false);
		_ui->imagesTreeView->header()->setHidden(true);
	});
	
	_scanner.loadSettings();
	_scanner.scan();
}