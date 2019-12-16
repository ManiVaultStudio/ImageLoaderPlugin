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
	_loader(ImageCollectionType::MultiPartSequence)
{
	_ui->setupUi(this);

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, &_scanner, &MultiPartImageSequenceScanner::setDirectory);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onLoadPushButtonClicked);
	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, &_loader, &ImageLoader::setDatasetName);

	connect(&_scanner, &MultiPartImageSequenceScanner::directoryChanged, this, &MultiPartImageSequenceWidget::onDirectoryChanged);
	connect(&_scanner, &MultiPartImageSequenceScanner::beginScan, this, &MultiPartImageSequenceWidget::onBeginScan);
	connect(&_scanner, &MultiPartImageSequenceScanner::endScan, this, &MultiPartImageSequenceWidget::onEndScan);
	
	connect(&_loader, &ImageLoader::beginLoad, this, &MultiPartImageSequenceWidget::onBeginLoad);
	connect(&_loader, &ImageLoader::endLoad, this, &MultiPartImageSequenceWidget::onEndLoad);
	connect(&_loader, &ImageLoader::datasetNameChanged, this, &MultiPartImageSequenceWidget::onDatasetNameChanged);

	connect(&_scanner, &MultiPartImageSequenceScanner::message, this, &MultiPartImageSequenceWidget::message);
	connect(&_loader, &ImageLoader::message, this, &MultiPartImageSequenceWidget::message);

	connect(&_scanner, &MultiPartImageSequenceScanner::settingsChanged, this, &MultiPartImageSequenceWidget::onScannerSettingsChanged);
	connect(&_loader, &ImageLoader::settingsChanged, this, &MultiPartImageSequenceWidget::onLoaderSettingsChanged);

	_ui->subsampleSettingsWidget->initialize(&_loader.subsampleImageSettings());
	_ui->colorSettingsWidget->initialize(&_loader.colorSettings());

	_ui->imagesLabel->setVisible(false);
	_ui->imagesListWidget->setVisible(false);

	_scanner.loadSettings();
}

void MultiPartImageSequenceWidget::onScannerSettingsChanged()
{
}

void MultiPartImageSequenceWidget::onLoaderSettingsChanged()
{
}

void MultiPartImageSequenceWidget::onPickDirectory()
{
	const auto initialDirectory = _scanner.directory();
	const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose multipart image sequence directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_scanner.setDirectory(pickedDirectory);
	}
}

void MultiPartImageSequenceWidget::onDirectoryChanged(const QString& directory)
{
	const auto validDirectory = !directory.isEmpty() && QDir(directory).exists();

	if (validDirectory) {
		_ui->directoryLineEdit->setText(directory);
		_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
	}

	_ui->datasetNameLabel->setEnabled(validDirectory);
	_ui->datasetNameLineEdit->setEnabled(validDirectory);

	_scanner.scan();
}

void MultiPartImageSequenceWidget::onLoadPushButtonClicked()
{
	_loader.load(_scanner.scanned());

	_ui->loadPushButton->setEnabled(false);
}

void MultiPartImageSequenceWidget::onDatasetNameChanged(const QString& dataSetName)
{
	_ui->loadPushButton->setEnabled(!dataSetName.isEmpty() && _scanner.scanned().loadable());
}

void MultiPartImageSequenceWidget::onBeginScan()
{
}

void MultiPartImageSequenceWidget::onEndScan(const ImageCollections& scannedImageCollections)
{
	const auto loadable = _scanner.scanned().loadable();

	_ui->imagesLabel->setEnabled(loadable);
	_ui->imagesListWidget->setEnabled(false);
	_ui->datasetNameLabel->setEnabled(loadable);
	_ui->datasetNameLineEdit->setEnabled(loadable);
	_ui->loadPushButton->setEnabled(loadable);

	_ui->imagesListWidget->clear();
}

void MultiPartImageSequenceWidget::onBeginLoad()
{
}

void MultiPartImageSequenceWidget::onEndLoad(Payload& payload)
{
	_ui->loadPushButton->setEnabled(false);
	_ui->loadPushButton->setText("Load");

	_imageLoaderPlugin->addImages(payload);
}