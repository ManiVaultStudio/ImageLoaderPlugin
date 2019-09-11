#include "ImageStackWidget.h"

#include "ui_imageStackWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ImageStackWidget::ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageStackWidget>() },
	_scanner(),
	_scanned(ImageCollectionType::Stack),
	_loader(ImageCollectionType::Stack)
{
	_ui->setupUi(this);
	
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageStackWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &ImageStackWidget::onLoadPushButtonClicked);
	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, &_scanned, &ImageCollections::setDatasetName);

	connect(&_scanner, &ImageStackScanner::directoryChanged, this, &ImageStackWidget::onDirectoryChanged);
	connect(&_scanner, &ImageStackScanner::beginScan, this, &ImageStackWidget::onBeginScan);
	connect(&_scanner, &ImageStackScanner::endScan, this, &ImageStackWidget::onEndScan);
	
	connect(&_scanner, &ImageStackScanner::message, this, &ImageStackWidget::message);
	connect(&_loader, &ImageCollectionsLoader::message, this, &ImageStackWidget::message);

	connect(&_scanned, &ImageCollections::datasetNameChanged, this, &ImageStackWidget::onDatasetNameChanged);

	_ui->subsampleImageSettingsWidget->initialize(&_loader.subsampleImageSettings());

	emit _scanner.directoryChanged(_scanner.directory());
}

void ImageStackWidget::onPickDirectory()
{
	const auto initialDirectory = _scanner.directory();
	const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image stack directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_scanner.setDirectory(pickedDirectory);
	}
}

void ImageStackWidget::onDirectoryChanged(const QString& directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	_scanner.scan();
}

void ImageStackWidget::onLoadPushButtonClicked()
{
	ImageCollections scanned = _scanned;

	foreach(QString key, scanned.map().keys()) {
		if (key != _ui->stacksComboBox->currentText()) {
			scanned.map().remove(key);
		}
	}

	_loader.load(scanned);

	_ui->loadPushButton->setEnabled(false);
}

void ImageStackWidget::onDatasetNameChanged(const QString& text)
{
	_ui->loadPushButton->setEnabled(!text.isEmpty() && _scanned.map().size() > 0);
}

void ImageStackWidget::onBeginScan()
{
}

void ImageStackWidget::onEndScan(ImageCollections& imageCollections)
{
	_scanned = imageCollections;

	// qDebug() << imageCollections;

	const auto canLoad = _scanned.map().size() > 0;

	_ui->datasetNameLabel->setEnabled(canLoad);
	_ui->datasetNameLineEdit->setEnabled(canLoad);
	_ui->stacksComboBox->setEnabled(canLoad);
	_ui->loadPushButton->setEnabled(canLoad);

	_ui->stacksComboBox->clear();
	_ui->stacksComboBox->addItems(_scanned.map().keys());
}

void ImageStackWidget::onBeginLoad()
{
	_ui->loadPushButton->setText("Loading");
}

void ImageStackWidget::onEndLoad(ImageCollections& imageCollections)
{
	_ui->loadPushButton->setEnabled(false);
	_ui->loadPushButton->setText("Load");

	_imageLoaderPlugin->addDataSet(imageCollections);
}
