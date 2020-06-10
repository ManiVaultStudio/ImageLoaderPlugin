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

	auto& imageCollectionsModel				= _imageLoaderPlugin->imageCollectionsModel();
	auto& imageCollectionsSelectionModel	= imageCollectionsModel.selectionModel();
	auto& filterModel						= _imageLoaderPlugin->imageCollectionsFilterModel();

	_ui->imageCollectionsTreeView->setModel(&_imageLoaderPlugin->imageCollectionsFilterModel());
	_ui->imageCollectionsTreeView->setSelectionModel(&imageCollectionsModel.selectionModel());

	


	// Column visibility
	for (int column = ult(ImageCollection::Image::Column::Start); column <= ult(ImageCollection::Image::Column::End); column++)
		_ui->imageCollectionsTreeView->header()->hideSection(column);

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

	
	//_ui->imagesTreeView->setSelectionModel(&imageCollectionsModel.selectionModel());
	_ui->imagesTreeView->header()->setHidden(true);

	QObject::connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString text) {
		_scanner.setDirectory(text);
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

	QObject::connect(_ui->searchFilterLineEdit, &QLineEdit::textChanged, [this](QString text) {
		_imageLoaderPlugin->imageCollectionsFilterModel().setFilter(text);
	});

	QObject::connect(_ui->loadAsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [&](int currentIndex) {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (!selectedRows.isEmpty()) {
			imageCollectionsModel.setData(filterModel.mapToSource(selectedRows.first()).siblingAtColumn(ult(ImageCollection::Column::Type)), currentIndex);
		}
	});

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
	
	
	QObject::connect(_ui->selectAllPushButton, &QPushButton::clicked, [&]() {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (!selectedRows.isEmpty())
			imageCollectionsModel.selectAll(filterModel.mapToSource(selectedRows.first()));
	});

	QObject::connect(_ui->selectNonePushButton, &QPushButton::clicked, [&]() {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (!selectedRows.isEmpty())
			imageCollectionsModel.selectNone(filterModel.mapToSource(selectedRows.first()));
	});

	QObject::connect(_ui->invertSelectionPushButton, &QPushButton::clicked, [&]() {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (!selectedRows.isEmpty())
			imageCollectionsModel.invertSelection(filterModel.mapToSource(selectedRows.first()));
	});

	QObject::connect(_ui->selectPercentagePushButton, &QPushButton::clicked, [&]() {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (!selectedRows.isEmpty())
			imageCollectionsModel.selectPercentage(filterModel.mapToSource(selectedRows.first()), 0.01f * _ui->selectPercentageDoubleSpinBox->value());
	});

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::modelReset, [this, &imageCollectionsModel]() {
		_ui->imagesTreeView->setModel(nullptr);

		_ui->selectAllPushButton->setEnabled(false);
		_ui->selectNonePushButton->setEnabled(false);
		_ui->invertSelectionPushButton->setEnabled(false);
		_ui->selectPercentageDoubleSpinBox->setEnabled(false);
		_ui->selectPercentagePushButton->setEnabled(false);
	});
	
	auto updateSelectionButtons = [&](const QModelIndex& index) {
		const auto noImages			= imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::NoImages)), Qt::EditRole).toInt();
		const auto noSelectedImages	= imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages)), Qt::EditRole).toInt();

		_ui->selectAllPushButton->setEnabled(noSelectedImages != noImages);
		_ui->selectNonePushButton->setEnabled(noSelectedImages > 0);
		_ui->invertSelectionPushButton->setEnabled(true);
		_ui->selectPercentageDoubleSpinBox->setEnabled(noImages > 1);
		_ui->selectPercentagePushButton->setEnabled(noImages > 1);
	};

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, [this, &imageCollectionsModel, &imageCollectionsSelectionModel, &filterModel, updateSelectionButtons](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles) {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (!selectedRows.isEmpty() && topLeft.row() == selectedRows.first().row()) {
			switch (static_cast<ImageCollection::Column>(topLeft.column()))
			{
				case ImageCollection::Column::NoSelectedImages:
					updateSelectionButtons(filterModel.mapToSource(selectedRows.first()));
					break;

				default:
					break;
			}
		}
	});
	
	QObject::connect(_ui->imageCollectionsTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, [this, &imageCollectionsModel, &imageCollectionsSelectionModel, &filterModel, updateSelectionButtons](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (!selectedRows.isEmpty()) {
			const auto index = filterModel.mapToSource(selectedRows.first());

			_ui->imagesLabel->setEnabled(true);
			_ui->imagesTreeView->setEnabled(true);
			_ui->imagesTreeView->setModel(&imageCollectionsModel);
			_ui->imagesTreeView->setRootIndex(index);
			_ui->imagesTreeView->header()->setHidden(false);

			for (int column = ult(ImageCollection::Column::Start); column <= ult(ImageCollection::Column::End); column++)
				_ui->imagesTreeView->header()->hideSection(column);

			_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::ShouldLoad), QHeaderView::Fixed);
			_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::FileName), QHeaderView::Interactive);
			_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::DimensionName), QHeaderView::Interactive);
			_ui->imagesTreeView->header()->setSectionResizeMode(ult(ImageCollection::Image::Column::FileName), QHeaderView::Interactive);

			_ui->imagesTreeView->header()->setMinimumSectionSize(20);
			_ui->imagesTreeView->header()->resizeSection(ult(ImageCollection::Image::Column::ShouldLoad), 20);
			_ui->imagesTreeView->header()->resizeSection(ult(ImageCollection::Image::Column::FileName), 200);
			_ui->imagesTreeView->header()->resizeSection(ult(ImageCollection::Image::Column::DimensionName), 200);
			_ui->imagesTreeView->header()->resizeSection(ult(ImageCollection::Image::Column::FilePath), 200);
			
			updateSelectionButtons(index);
		}
	});

	_scanner.loadSettings();
	_scanner.scan();
}