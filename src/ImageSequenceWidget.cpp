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
	_loader()
{
	_ui->setupUi(this);
	
	const auto width	= _loader.setting("ImageWidth", QVariant(28)).toInt();
	const auto height	= _loader.setting("ImageHeight", QVariant(28)).toInt();

	_scanner.setImageSize(QSize(width, height));

	_ui->imageTypeComboBox->setCurrentText(_scanner.imageType());
	_ui->imageWidthSpinBox->setValue(_scanner.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_scanner.imageSize().height());

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, this, &ImageSequenceWidget::onImageTypeChanged);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onPickDirectory);
	connect(_ui->imageWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageHeightChanged);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onLoadSequence);
	connect(&_scanner, &ImageSequenceScanner::directoryChanged, this, &ImageSequenceWidget::onDirectoryChanged);
	connect(&_scanner, &ImageSequenceScanner::becameDirty, this, &ImageSequenceWidget::onBecameDirty);
	connect(&_scanner, &ImageSequenceScanner::beginScan, this, &ImageSequenceWidget::onBeginScan);
	connect(&_scanner, &ImageSequenceScanner::endScan, this, &ImageSequenceWidget::onEndScan);
	connect(&_loader, &ImageSequenceLoader::beginLoad, this, &ImageSequenceWidget::onBeginLoad);
	connect(&_loader, &ImageSequenceLoader::endLoad, this, &ImageSequenceWidget::onEndLoad);

	connect(&_loader, &ImageSequenceLoader::message, this, &ImageSequenceWidget::message);
	connect(&_scanner, &ImageSequenceScanner::message, this, &ImageSequenceWidget::message);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
	_ui->imageTypeComboBox->addItem("tif");

	_ui->resampleImageSettingsWidget->initialize(&_loader.resampleImageSettings());

	emit _scanner.directoryChanged(_scanner.directory());
}

ImageSequenceWidget::~ImageSequenceWidget()
{
}

ImageSequenceLoader & ImageSequenceWidget::loader()
{
	return _loader;
}

void ImageSequenceWidget::onBecameDirty()
{
	_scanner.scan();
}

void ImageSequenceWidget::onBeginScan()
{
}

void ImageSequenceWidget::onEndScan(QStringList& imageFilePaths)
{
	_ui->loadSequencePushButton->setEnabled(imageFilePaths.size() > 0);
}

void ImageSequenceWidget::onDirectoryChanged(const QString& directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	_scanner.scan();
}

void ImageSequenceWidget::onLoadSequence()
{
	_loader.load();

	_ui->loadSequencePushButton->setEnabled(false);
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
	_ui->loadSequencePushButton->setText("Loading");
}

void ImageSequenceWidget::onEndLoad(FloatVector& pointsData)
{
	/*
	_imageLoaderPlugin->addSequence(ImageCollectionType::Sequence, _ui->datasetNameLineEdit->text(), _loader.imageSize(), _loader.noImages(), _loader.noDimensions(), pointsData, _loader.dimensionNames());

	_ui->loadSequencePushButton->setText("Load");
	*/
}