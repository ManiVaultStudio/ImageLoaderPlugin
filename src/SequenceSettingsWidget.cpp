#include "SequenceSettingsWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_SequenceSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

SequenceSettingsWidget::SequenceSettingsWidget(QWidget* parent) :
	QWidget(parent),
	_imageLoaderPlugin(nullptr),
	_ui{ std::make_unique<Ui::SequenceSettingsWidget>() },
	_scanner(),
	_loader(nullptr, ImageData::Type::Sequence)
{
	_ui->setupUi(this);
}

SequenceSettingsWidget::~SequenceSettingsWidget() = default;

void SequenceSettingsWidget::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_scanner.setImageLoaderPlugin(imageLoaderPlugin);

	_ui->sequencesTreeView->setModel(&_imageLoaderPlugin->imageCollectionsModel());
	//_ui->imagesTreeView->setModel(&_imageLoaderPlugin->imageCollectionsModel());

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString directory) {
		_scanner.setDirectory(directory);
	});

	connect(&_scanner, &ImageSequenceScanner::directoryChanged, [this](const QString& directory) {
		_ui->directoryLineEdit->blockSignals(true);
		_ui->directoryLineEdit->setText(directory);
		_ui->directoryLineEdit->blockSignals(false);
	});

	connect(_ui->directoryPushButton, &QPushButton::clicked, [this]() {
		const auto initialDirectory	= _scanner.directory();
		const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

		if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
			_scanner.setDirectory(pickedDirectory);
		}
	});

	connect(_ui->scanPushButton, &QPushButton::clicked, [this]() {
		_scanner.scan();
	});

	connect(&_scanner, &ImageSequenceScanner::settingsChanged, [this]() {
		_ui->scanPushButton->setEnabled(true);
	});

	QObject::connect(&_scanner, &ImageSequenceScanner::beginScan, this, [this]() {
		_ui->scanPushButton->setText("Scanning...");
	}, Qt::QueuedConnection);

	/*
	QObject::connect(&_scanner, &ImageSequenceScanner::endScan, this, [this](std::shared_ptr<Scanned> scanned) {
		const auto loadable = scanned->loadable();

		_ui->datasetNameLabel->setEnabled(loadable);
		_ui->datasetNameLineEdit->setEnabled(loadable);

		_ui->scanPushButton->setText("Scan");
		_ui->scanPushButton->setEnabled(false);
	}, Qt::QueuedConnection);
	*/

	/*
	connect(&_loader, &ImageLoader::settingsChanged, [this]() {
		const auto enableLoad = _scanner.scanned()->loadable();
	});
	*/

	QObject::connect(&_loader, &ImageLoader::beginLoad, this, [this]() {
	}, Qt::QueuedConnection);

	_scanner.loadSettings();
}