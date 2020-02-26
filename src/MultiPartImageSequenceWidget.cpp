#include "MultiPartImageSequenceWidget.h"

#include "ui_MultiPartImageSequenceWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

#include "ImageLoaderPlugin.h"

MultiPartImageSequenceWidget::MultiPartImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::MultiPartImageSequenceWidget>() },
	_scanner(),
	_loader(imageLoaderPlugin, ImageCollectionType::MultiPartSequence)
{
	_ui->setupUi(this);

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString directory) {
		_scanner.setDirectory(directory);
	});

	connect(_ui->directoryPushButton, &QPushButton::clicked, [this]() {
		const auto initialDirectory = _scanner.directory();
		const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose multipart image sequence directory", initialDirectory);

		if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
			_scanner.setDirectory(pickedDirectory);
		}
	});

	connect(_ui->loadPushButton, &QPushButton::clicked, [this]() {
		_loader.load(_scanner.scanned());
		_ui->loadPushButton->setEnabled(false);
	});

	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, [this](QString datasetName) {
		_loader.setDatasetName(datasetName);
	});

	connect(&_scanner, &MultiPartImageSequenceScanner::directoryChanged, [this](const QString& directory) {
		const auto validDirectory = !directory.isEmpty() && QDir(directory).exists();

		if (validDirectory) {
			_ui->directoryLineEdit->setText(directory);
			_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
		}

		_ui->datasetNameLabel->setEnabled(validDirectory);
		_ui->datasetNameLineEdit->setEnabled(validDirectory);
	});

	connect(&_scanner, &MultiPartImageSequenceScanner::endScan, this, [this](std::shared_ptr<Scanned> scanned) {
		const auto loadable = scanned->loadable();

		_ui->imagesLabel->setEnabled(loadable);
		_ui->imagesListWidget->setEnabled(false);
		_ui->datasetNameLabel->setEnabled(loadable);
		_ui->datasetNameLineEdit->setEnabled(loadable);
		_ui->loadPushButton->setEnabled(loadable);

		_ui->imagesListWidget->clear();
	}, Qt::QueuedConnection);

	connect(&_loader, &ImageLoader::endLoad, this, [this](std::shared_ptr<Payload> payload) {
		_ui->loadPushButton->setEnabled(false);
		_ui->loadPushButton->setText("Load");
	}, Qt::QueuedConnection);

	connect(&_loader, &ImageLoader::datasetNameChanged, [this](const QString& datasetName) {
		_ui->loadPushButton->setEnabled(!datasetName.isEmpty() && _scanner.scanned()->loadable());
	});

	_ui->subsampleSettingsWidget->initialize(&_loader.subsampleImageSettings());
	_ui->colorSettingsWidget->initialize(&_loader.colorSettings());

	_ui->imagesLabel->setVisible(false);
	_ui->imagesListWidget->setVisible(false);

	_scanner.loadSettings();
}

MultiPartImageSequenceWidget::~MultiPartImageSequenceWidget() = default;

void MultiPartImageSequenceWidget::showEvent(QShowEvent* showEvent)
{
	_scanner.scan();

	QWidget::showEvent(showEvent);
}