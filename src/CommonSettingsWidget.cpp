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

	auto& imageCollectionsModel	= _imageLoaderPlugin->imageCollectionsModel();
	auto& imagesModel			= _imageLoaderPlugin->imagesModel();

	_ui->imageCollectionsTreeView->setModel(&imageCollectionsModel);
	_ui->imageCollectionsTreeView->setSelectionModel(&imageCollectionsModel.selectionModel());

	_ui->imagesTreeView->setModel(&imagesModel);

	_scanner.scan();

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString directory) {
		_scanner.setDirectory(directory);
	});

	connect(&_scanner, &ImageCollectionScanner::directoryChanged, [this](const QString& directory) {
		_ui->directoryLineEdit->blockSignals(true);
		_ui->directoryLineEdit->setText(directory);
		_ui->directoryLineEdit->blockSignals(false);

		_scanner.scan();
	});

	connect(_ui->directoryPushButton, &QPushButton::clicked, [this]() {
		const auto initialDirectory	= _scanner.directory();
		const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

		if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
			_scanner.setDirectory(pickedDirectory);
		}
	});

	QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [this, &imageCollectionsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty())
			qDebug() << selectedRows;
	});

	/*
	connect(&_loader, &ImageLoader::settingsChanged, [this]() {
		const auto enableLoad = _scanner.scanned()->loadable();
	});
	*/

	_scanner.loadSettings();
}