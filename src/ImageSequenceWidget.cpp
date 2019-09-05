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
	
	const auto width	= _imageSequence.setting("ImageWidth", QVariant(28)).toInt();
	const auto height	= _imageSequence.setting("ImageHeight", QVariant(28)).toInt();

	_imageSequence.setImageSize(QSize(width, height));

	_ui->imageTypeComboBox->setCurrentText(_imageSequence.imageType());
	_ui->imageWidthSpinBox->setValue(_imageSequence.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_imageSequence.imageSize().height());

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, this, &ImageSequenceWidget::onImageTypeChanged);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onPickDirectory);
	connect(_ui->scanPushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onScan);
	connect(_ui->imageWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImageSequenceWidget::onImageHeightChanged);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &ImageSequenceWidget::onLoadSequence);
	connect(&_imageSequence, &ImageSequence::directoryChanged, this, &ImageSequenceWidget::onDirectoryChanged);
	connect(&_imageSequence, &ImageSequence::becameDirty, this, &ImageSequenceWidget::onBecameDirty);
	connect(&_imageSequence, &ImageSequence::beginScan, this, &ImageSequenceWidget::onBeginScan);
	connect(&_imageSequence, &ImageSequence::endScan, this, &ImageSequenceWidget::onEndScan);
	connect(&_imageSequence, &ImageSequence::beginLoad, this, &ImageSequenceWidget::onBeginLoad);
	connect(&_imageSequence, &ImageSequence::endLoad, this, &ImageSequenceWidget::onEndLoad);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
	_ui->imageTypeComboBox->addItem("tif");

	const auto directory = _imageSequence.setting("Directory", "").toString();

	if (QDir(directory).exists()) {
		_imageSequence.setDirectory(directory);
	}

	_ui->resampleImageSettingsWidget->initialize(&_imageSequence.resampleImageSettings());
}

ImageSequenceWidget::~ImageSequenceWidget()
{
}

ImageSequence & ImageSequenceWidget::imageSequence()
{
	return _imageSequence;
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
	_ui->loadSequencePushButton->setEnabled(_imageSequence.noImages() > 0);

	_ui->scanPushButton->setText("Scan");
}

void ImageSequenceWidget::onDirectoryChanged(const QString& directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	_imageSequence.scan();

	_imageSequence.setSetting("Directory", directory);
}

void ImageSequenceWidget::onLoadSequence()
{
	_imageSequence.load();

	_ui->loadSequencePushButton->setEnabled(false);
}

void ImageSequenceWidget::onImageWidthChanged(int imageWidth)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));

	_imageSequence.setSetting("ImageWidth", imageWidth);
}

void ImageSequenceWidget::onImageHeightChanged(int imageHeight)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));

	_imageSequence.setSetting("ImageHeight", imageHeight);
}

void ImageSequenceWidget::onScan()
{
	_imageSequence.scan();
}

void ImageSequenceWidget::onPickDirectory()
{
	const auto initialDirectory = _imageSequence.setting("Directory").toString();
	const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_imageSequence.setDirectory(pickedDirectory);

		_imageSequence.scan();
	}
}

void ImageSequenceWidget::onImageTypeChanged(const QString & imageType)
{
	_imageSequence.setImageType(_ui->imageTypeComboBox->currentText());
}

void ImageSequenceWidget::onBeginLoad()
{
	_ui->loadSequencePushButton->setText("Loading");
}

void ImageSequenceWidget::onEndLoad(FloatVector& pointsData)
{
	_imageLoaderPlugin->addSequence(ImageCollection::Type::Sequence, _ui->datasetNameLineEdit->text(), _imageSequence.imageSize(), _imageSequence.noImages(), _imageSequence.noDimensions(), pointsData, _imageSequence.dimensionNames());

	_ui->loadSequencePushButton->setText("Load");
}