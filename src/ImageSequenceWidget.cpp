#include "ImageSequenceWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_ImageSequenceWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

ImageSequenceWidget::ImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageSequenceWidget>() }
{
	_ui->setupUi(this);
	
	const auto width	= _loader.setting("ImageWidth", QVariant(28)).toInt();
	const auto height	= _loader.setting("ImageHeight", QVariant(28)).toInt();

	_loader.setImageSize(QSize(width, height));

	_ui->imageTypeComboBox->setCurrentText(_loader.imageType());
	_ui->imageWidthSpinBox->setValue(_loader.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_loader.imageSize().height());

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, this, &ImageSequenceWidget::onImageTypeChanged);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onPickDirectory);
	connect(_ui->scanPushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onScan);
	connect(_ui->imageWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageHeightChanged);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onLoadSequence);
	connect(&_loader, &ImageSequenceLoader::directoryChanged, this, &ImageSequenceWidget::onDirectoryChanged);
	connect(&_loader, &ImageSequenceLoader::becameDirty, this, &ImageSequenceWidget::onBecameDirty);
	connect(&_loader, &ImageSequenceLoader::beginScan, this, &ImageSequenceWidget::onBeginScan);
	connect(&_loader, &ImageSequenceLoader::endScan, this, &ImageSequenceWidget::onEndScan);
	connect(&_loader, &ImageSequenceLoader::beginLoad, this, &ImageSequenceWidget::onBeginLoad);
	connect(&_loader, &ImageSequenceLoader::endLoad, this, &ImageSequenceWidget::onEndLoad);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
	_ui->imageTypeComboBox->addItem("tif");

	const auto directory = _loader.setting("Directory", "").toString();

	if (QDir(directory).exists()) {
		_loader.setDirectory(directory);
	}

	_ui->resampleImageSettingsWidget->initialize(&_loader.resampleImageSettings());
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
	_ui->scanPushButton->setEnabled(true);
}

void ImageSequenceWidget::onBeginScan()
{
	_ui->scanPushButton->setText("Scanning");

	_ui->scanPushButton->setEnabled(false);
}

void ImageSequenceWidget::onEndScan()
{
	_ui->loadSequencePushButton->setEnabled(_loader.noImages() > 0);

	_ui->scanPushButton->setText("Scan");
}

void ImageSequenceWidget::onDirectoryChanged(const QString& directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	_loader.scan();

	_loader.setSetting("Directory", directory);
}

void ImageSequenceWidget::onLoadSequence()
{
	_loader.load();

	_ui->loadSequencePushButton->setEnabled(false);
}

void ImageSequenceWidget::onImageWidthChanged(int imageWidth)
{
	_loader.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));

	_loader.setSetting("ImageWidth", imageWidth);
}

void ImageSequenceWidget::onImageHeightChanged(int imageHeight)
{
	_loader.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));

	_loader.setSetting("ImageHeight", imageHeight);
}

void ImageSequenceWidget::onScan()
{
	_loader.scan();
}

void ImageSequenceWidget::onPickDirectory()
{
	const auto initialDirectory = _loader.setting("Directory").toString();
	const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_loader.setDirectory(pickedDirectory);

		_loader.scan();
	}
}

void ImageSequenceWidget::onImageTypeChanged(const QString & imageType)
{
	_loader.setImageType(_ui->imageTypeComboBox->currentText());
}

void ImageSequenceWidget::onBeginLoad()
{
	_ui->loadSequencePushButton->setText("Loading");
}

void ImageSequenceWidget::onEndLoad(FloatVector& pointsData)
{
	_imageLoaderPlugin->addSequence(ImageCollectionType::Sequence, _ui->datasetNameLineEdit->text(), _loader.imageSize(), _loader.noImages(), _loader.noDimensions(), pointsData, _loader.dimensionNames());

	_ui->loadSequencePushButton->setText("Load");
}