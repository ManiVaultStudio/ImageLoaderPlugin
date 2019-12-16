#include "ImageStackWidget.h"

#include "ui_ImageStackWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ImageStackWidget::ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageStackWidget>() },
	_scanner(),
	_loader(ImageCollectionType::Stack)
{
	_ui->setupUi(this);

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, &_scanner, &ImageStackScanner::setDirectory);
	connect(_ui->pickDirectoryPushButton, &QPushButton::clicked, this, &ImageStackWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &ImageStackWidget::onLoadPushButtonClicked);
	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, &_loader, &ImageLoader::setDatasetName);

	connect(&_scanner, &ImageStackScanner::directoryChanged, this, &ImageStackWidget::onDirectoryChanged);
	connect(&_scanner, &ImageStackScanner::previousDirectoriesChanged, [&](const QStringList& previousDirectories) { _ui->previousDirectoriesComboBox->clear(); _ui->previousDirectoriesComboBox->addItems(previousDirectories);  });
	connect(&_scanner, &ImageStackScanner::beginScan, this, &ImageStackWidget::onBeginScan);
	connect(&_scanner, &ImageStackScanner::endScan, this, &ImageStackWidget::onEndScan);
	
	connect(&_loader, &ImageLoader::beginLoad, this, &ImageStackWidget::onBeginLoad);
	connect(&_loader, &ImageLoader::endLoad, this, &ImageStackWidget::onEndLoad);
	connect(&_loader, &ImageLoader::datasetNameChanged, this, &ImageStackWidget::onDatasetNameChanged);

	connect(&_scanner, &ImageStackScanner::message, this, &ImageStackWidget::message);
	connect(&_loader, &ImageLoader::message, this, &ImageStackWidget::message);

	connect(&_loader.subsampleImageSettings(), &SubsampleSettings::settingsChanged, this, &ImageStackWidget::onSubsampleImageSettingsChanged);

	connect(&_scanner, &ImageStackScanner::settingsChanged, this, &ImageStackWidget::onScannerSettingsChanged);
	connect(&_loader, &ImageLoader::settingsChanged, this, &ImageStackWidget::onLoaderSettingsChanged);

	_ui->subsampleSettingsWidget->initialize(&_loader.subsampleImageSettings());
	_ui->colorSettingsWidget->initialize(&_loader.colorSettings());

	_ui->previousDirectoriesComboBox->setVisible(false);

	_scanner.loadSettings();
}

void ImageStackWidget::onScannerSettingsChanged()
{
	_scanner.scan();

	_ui->loadPushButton->setEnabled(_scanner.scanned().loadable());
}

void ImageStackWidget::onLoaderSettingsChanged()
{
	_ui->loadPushButton->setEnabled(_scanner.scanned().loadable());
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
	foreach(QString key, _scanner.scanned().map().keys()) {
		if (key != _ui->stacksComboBox->currentText()) {
			_scanner.scanned().map().remove(key);
		}
	}

	_loader.load(_scanner.scanned());

	_ui->loadPushButton->setEnabled(false);
}

void ImageStackWidget::onDatasetNameChanged(const QString& dataSetName)
{
	_ui->loadPushButton->setEnabled(!dataSetName.isEmpty() && _scanner.scanned().loadable());
}

void ImageStackWidget::onBeginScan()
{
}

void ImageStackWidget::onEndScan(const ImageCollections& scannedImageCollections)
{
	const auto loadable = _scanner.scanned().loadable();

	_ui->datasetNameLabel->setEnabled(loadable);
	_ui->datasetNameLineEdit->setEnabled(loadable);
	_ui->stacksComboBox->setEnabled(loadable);
	_ui->loadPushButton->setEnabled(loadable);

	_ui->stacksComboBox->clear();
	_ui->stacksComboBox->addItems(_scanner.scanned().map().keys());
}

void ImageStackWidget::onBeginLoad()
{
	_ui->loadPushButton->setText("Loading");
}

void ImageStackWidget::onEndLoad(Payload& payload)
{
	_ui->loadPushButton->setEnabled(false);
	_ui->loadPushButton->setText("Load");

	_imageLoaderPlugin->addImages(payload);
}

void ImageStackWidget::onSubsampleImageSettingsChanged()
{
	const auto loadable = _scanner.scanned().loadable();

	_ui->loadPushButton->setEnabled(loadable);
}
