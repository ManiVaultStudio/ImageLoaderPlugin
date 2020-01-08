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
	
	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
	_ui->imageTypeComboBox->addItem("tif");
	_ui->imageTypeComboBox->addItem("tiff");

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [=](QString directory)
	{
		_scanner.setDirectory(directory);
	});

	connect(&_scanner, &ImageSequenceScanner::directoryChanged, [=](const QString& directory) {
		_ui->directoryLineEdit->blockSignals(true);
		_ui->directoryLineEdit->setText(directory);
		_ui->directoryLineEdit->blockSignals(false);
		_ui->datasetNameLineEdit->setEnabled(!directory.isEmpty());
		_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
	});

	connect(_ui->directoryPushButton, &QPushButton::clicked, [=]() {
		const auto initialDirectory = _loader.setting("Directory").toString();
		const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

		if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
			_scanner.setDirectory(pickedDirectory);
		}
	});

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, [=](QString imageType)
	{
		_scanner.setImageType(imageType);
	});
	
	connect(&_scanner, &ImageSequenceScanner::imageTypeChanged, [=](const QString& imageType) {
		_ui->imageTypeComboBox->blockSignals(true);
		_ui->imageTypeComboBox->setCurrentText(imageType);
		_ui->imageTypeComboBox->blockSignals(false);
	});

	connect(&_scanner, &ImageSequenceScanner::imageSizeChanged, [=](const QSize& imageSize) {
		_ui->directoryLineEdit->blockSignals(true);
		_ui->imageWidthSpinBox->setValue(imageSize.width());
		_ui->imageHeightSpinBox->setValue(imageSize.height());
		_ui->directoryLineEdit->blockSignals(false);
	});

	connect(_ui->imageWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int width) {
		_scanner.setImageWidth(width);
	});

	connect(_ui->imageHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int height) {
		_scanner.setImageHeight(height);
	});

	connect(&_loader, &ImageLoader::datasetNameChanged, [=](const QString& datasetName) {
		_ui->datasetNameLineEdit->blockSignals(true);
		_ui->datasetNameLineEdit->setText(datasetName);
		_ui->datasetNameLineEdit->blockSignals(false);
	});

	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, [=](const QString& text) {
		_loader.setDatasetName(text);
	});

	connect(_ui->squareCheckBox, &QCheckBox::stateChanged, [=](int state) {
		_scanner.setSquare(static_cast<bool>(state));
	});

	connect(&_scanner, &ImageSequenceScanner::squareChanged, [=](const bool& square) {
		_ui->imageHeightSpinBox->setEnabled(!square);
		_ui->squareCheckBox->setChecked(square);
	});

	connect(_ui->scanPushButton, &QPushButton::clicked, [=]() {
		_scanner.scan();
	});

	connect(_ui->loadPushButton, &QPushButton::clicked, [=]() {
		_loader.load(_scanner.scanned());
		_ui->loadPushButton->setEnabled(false);
	});

	connect(&_scanner, &ImageSequenceScanner::settingsChanged, [=]() {
		_ui->scanPushButton->setEnabled(true);
	});

	connect(&_loader, &ImageLoader::settingsChanged, [=]() {
		const auto enableLoad = _scanner.scanned()->loadable();
		_ui->loadPushButton->setEnabled(enableLoad);
	});

	connect(&_scanner, &ImageSequenceScanner::beginScan, [=]() {
		_ui->scanPushButton->setText("Scanning...");
	});

	connect(&_scanner, &ImageSequenceScanner::endScan, [=](std::shared_ptr<Scanned> scanned) {
		const auto loadable = scanned->loadable();

		_ui->datasetNameLabel->setEnabled(loadable);
		_ui->datasetNameLineEdit->setEnabled(loadable);
		_ui->loadPushButton->setEnabled(loadable);

		_ui->scanPushButton->setText("Scan");
		_ui->scanPushButton->setEnabled(false);
	});

	connect(&_loader, &ImageLoader::beginLoad, [=]() {
		_ui->loadPushButton->setText("Loading...");
	});

	connect(&_loader, &ImageLoader::endLoad, [=](std::shared_ptr<Payload> payload) {
		_ui->loadPushButton->setEnabled(false);
		_ui->loadPushButton->setText("Load");

		_imageLoaderPlugin->addImages(payload);
	});
	
	connect(&_loader, &ImageLoader::message, this, &ImageSequenceWidget::message);
	connect(&_scanner, &ImageSequenceScanner::message, this, &ImageSequenceWidget::message);

	_ui->subsampleSettingsWidget->initialize(&_loader.subsampleImageSettings());
	_ui->colorSettingsWidget->initialize(&_loader.colorSettings());

	_scanner.loadSettings();
}

ImageSequenceWidget::~ImageSequenceWidget() = default;