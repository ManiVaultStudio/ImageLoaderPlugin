#include "ImageSequenceWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_ImageSequenceWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

ImageSequenceWidget::ImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageSequenceWidget>() },
	_scanner(),
	_loader(ImageCollectionType::Sequence)
{
	_ui->setupUi(this);
	
	const auto width	= _loader.setting("ImageWidth", QVariant(28)).toInt();
	const auto height	= _loader.setting("ImageHeight", QVariant(28)).toInt();

	_scanner.setImageSize(QSize(width, height));

	_ui->imageTypeComboBox->setCurrentText(_scanner.imageType());
	_ui->imageWidthSpinBox->setValue(_scanner.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_scanner.imageSize().height());

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, this, &ImageSequenceWidget::onImageTypeChanged);
	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, &_scanner, &ImageSequenceScanner::setDirectory);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onPickDirectory);
	connect(_ui->imageWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageHeightChanged);
	connect(_ui->scanPushButton, &QPushButton::clicked, &this->_scanner, &ImageSequenceScanner::scan);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onLoadPushButtonClicked);
	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, &_loader, &ImageLoader::setDatasetName);

	connect(&_scanner, &ImageSequenceScanner::directoryChanged, this, &ImageSequenceWidget::onDirectoryChanged);
	connect(&_scanner, &ImageSequenceScanner::beginScan, this, &ImageSequenceWidget::onBeginScan);
	connect(&_scanner, &ImageSequenceScanner::endScan, this, &ImageSequenceWidget::onEndScan);
	connect(&_loader, &ImageLoader::beginLoad, this, &ImageSequenceWidget::onBeginLoad);
	connect(&_loader, &ImageLoader::endLoad, this, &ImageSequenceWidget::onEndLoad);
	connect(&_loader, &ImageLoader::datasetNameChanged, this, &ImageSequenceWidget::onDatasetNameChanged);
	
	connect(&_loader, &ImageLoader::message, this, &ImageSequenceWidget::message);
	connect(&_scanner, &ImageSequenceScanner::message, this, &ImageSequenceWidget::message);

	connect(&_scanner, &ImageSequenceScanner::settingsChanged, this, &ImageSequenceWidget::onScannerSettingsChanged);
	connect(&_loader, &ImageLoader::settingsChanged, this, &ImageSequenceWidget::onLoaderSettingsChanged);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
	_ui->imageTypeComboBox->addItem("tif");
	_ui->imageTypeComboBox->addItem("tiff");

	_ui->subsampleSettingsWidget->initialize(&_loader.subsampleImageSettings());
	_ui->colorSettingsWidget->initialize(&_loader.colorSettings());

	_scanner.loadSettings();
}

void ImageSequenceWidget::onScannerSettingsChanged()
{
	_ui->scanPushButton->setEnabled(true);
}

void ImageSequenceWidget::onLoaderSettingsChanged()
{
	const auto enableLoad = _scanner.scanned()->loadable();

	_ui->loadPushButton->setEnabled(enableLoad);
}

void ImageSequenceWidget::onBeginScan()
{
	_ui->scanPushButton->setText("Scanning...");
}

void ImageSequenceWidget::onEndScan(std::shared_ptr<ImageCollections> scanned)
{
	const auto loadable = scanned->loadable();

	_ui->datasetNameLabel->setEnabled(loadable);
	_ui->datasetNameLineEdit->setEnabled(loadable);
	_ui->loadPushButton->setEnabled(loadable);

	_ui->scanPushButton->setText("Scan");
	_ui->scanPushButton->setEnabled(false);
}

void ImageSequenceWidget::onDirectoryChanged(const QString& directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
}

void ImageSequenceWidget::onLoadPushButtonClicked()
{
	_loader.load(_scanner.scanned());

	_ui->loadPushButton->setEnabled(false);
}

void ImageSequenceWidget::onDatasetNameChanged(const QString& dataSetName)
{
	_ui->loadPushButton->setEnabled(!dataSetName.isEmpty() &&  _scanner.scanned()->loadable());
}

void ImageSequenceWidget::onImageWidthChanged(int imageWidth)
{
	_scanner.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageSequenceWidget::onImageHeightChanged(int imageHeight)
{
	_scanner.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageSequenceWidget::onPickDirectory()
{
	const auto initialDirectory = _loader.setting("Directory").toString();
	const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_scanner.setDirectory(pickedDirectory);
	}
}

void ImageSequenceWidget::onImageTypeChanged(const QString & imageType)
{
	_scanner.setImageType(_ui->imageTypeComboBox->currentText());
}

void ImageSequenceWidget::onBeginLoad()
{
	_ui->loadPushButton->setText("Loading...");
}

void ImageSequenceWidget::onEndLoad(std::shared_ptr<Payload> payload)
{
	_ui->loadPushButton->setEnabled(false);
	_ui->loadPushButton->setText("Load");

	_imageLoaderPlugin->addImages(payload);
}