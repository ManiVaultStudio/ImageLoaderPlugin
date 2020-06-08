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
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::DatasetName));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::NoImages));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::NoSelectedImages));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::SourceSize));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::TargetSize));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::TargetWidth));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::TargetHeight));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::Type));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::SubsamplingEnabled));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::SubsamplingRatio));
	_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::SubsamplingFilter));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::Grayscale));
	//_ui->imageCollectionsTreeView->header()->hideSection(ult(ImageCollectionsModel::Column::Directory));

	//_ui->imageCollectionsTreeView->header()->setStretchLastSection(false);

	_ui->imagesTreeView->setModel(&imagesModel);
	_ui->imagesTreeView->setSelectionModel(&imagesModel.selectionModel());

	//_ui->imagesTreeView->header()->setResizeContentsPrecision(1);

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
		
	});

	QObject::connect(_ui->loadAsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &imageCollectionsModel](int currentIndex) {
		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty()) {
			imageCollectionsModel.setData(selectedRows.first().siblingAtColumn(ult(ImageCollectionsModel::Column::Type)), currentIndex);
		}
	});

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::rowsInserted, [this]() {
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::DatasetName));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::ImageType));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::NoImages));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::NoSelectedImages));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::SourceSize));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::TargetSize));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::TargetWidth));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::TargetHeight));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::Directory));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::Type));
		_ui->imageCollectionsTreeView->resizeColumnToContents(ult(ImageCollectionsModel::Column::ToGrayscale));
	});

	QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [this, &imageCollectionsModel, &imagesModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty()) {
			const auto firstRow = selectedRows.first();

			imagesModel.setImageCollection(const_cast<ImageCollection*>(imageCollectionsModel.imageCollection(firstRow.row())));

			//_ui->imagesTreeView->resizeColumnToContents(ult(ImagesModel::Column::ShouldLoad));
			//_ui->imagesTreeView->resizeColumnToContents(ult(ImagesModel::Column::FileName));
			//_ui->imagesTreeView->resizeColumnToContents(ult(ImagesModel::Column::DimensionName));
			//_ui->imagesTreeView->resizeColumnToContents(ult(ImagesModel::Column::FilePath));

			const auto imageCollectionType = imageCollectionsModel.data(firstRow.siblingAtColumn(ult(ImageCollectionsModel::Column::Type)), Qt::EditRole).toInt();

			_ui->loadAsComboBox->setCurrentIndex(imageCollectionType);
			_ui->imagesTreeView->setColumnHidden(ult(ImagesModel::Column::DimensionName), imageCollectionType != ImageData::Type::Stack);
		}
	});

	_scanner.loadSettings();
}