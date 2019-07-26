#include "ImageSequenceWidget.h"

#include "ui_ImageSequenceWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoader.h"

ImageSequenceWidget::ImageSequenceWidget(ImageLoader *imageLoader) :
	_imageLoaderPlugin(imageLoader),
	_ui{ std::make_unique<Ui::ImageSequenceWidget>() }
{
	_ui->setupUi(this);
	
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
	connect(&_imageSequence, &ImageSequence::message, this, &ImageSequenceWidget::onMessage);
	connect(&_imageSequence, &ImageSequence::becameDirty, this, &ImageSequenceWidget::onBecameDirty);
	connect(&_imageSequence, &ImageSequence::beginScan, this, &ImageSequenceWidget::onBeginScan);
	connect(&_imageSequence, &ImageSequence::endScan, this, &ImageSequenceWidget::onEndScan);
	connect(&_imageSequence, &ImageSequence::beginLoad, this, &ImageSequenceWidget::onBeginLoad);
	connect(&_imageSequence, &ImageSequence::endLoad, this, &ImageSequenceWidget::onEndLoad);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
	_ui->imageTypeComboBox->addItem("tif");

	// onPickDirectory();
}

ImageSequenceWidget::~ImageSequenceWidget()
{
}

void ImageSequenceWidget::onBecameDirty()
{
	_ui->scanPushButton->setEnabled(true);
}

void ImageSequenceWidget::onBeginScan()
{
	_ui->infoLineEdit->setText(QString("Scanning for image files..."));
	_ui->scanPushButton->setText("Scanning");
	_ui->scanPushButton->setEnabled(false);
}

void ImageSequenceWidget::onEndScan()
{
	if (_imageSequence.imageFilePaths().size() == 0) {
		_ui->infoLineEdit->setText("No images were found, try changing the directory, image type or dimensions");
	}
	else {
		_ui->infoLineEdit->setText(QString("Found %1 images").arg(_imageSequence.imageFilePaths().size()));
	}

	_ui->loadSequencePushButton->setEnabled(true);
	_ui->scanPushButton->setText("Scan");
}

void ImageSequenceWidget::onMessage(const QString &message)
{
	// qDebug() << message;

	_ui->infoLineEdit->setText(message);
}

void ImageSequenceWidget::onDirectoryChanged(const QString &directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
}

void ImageSequenceWidget::onLoadSequence()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Load);
	_imageSequence.start();

	_ui->loadSequencePushButton->setEnabled(false);
}

void ImageSequenceWidget::onImageWidthChanged(int imageWidth)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageSequenceWidget::onImageHeightChanged(int imageHeight)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageSequenceWidget::onScan()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
	_imageSequence.start();
}

void ImageSequenceWidget::onPickDirectory()
{
	const auto _directory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory");

	if (!_directory.isNull() || !_directory.isEmpty()) {
		_imageSequence.setDirectory(_directory);

		_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
		_imageSequence.start();
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

void ImageSequenceWidget::onEndLoad()
{
	_imageLoaderPlugin->addSequence(_ui->datasetNameLineEdit->text(), this->_imageSequence.noDimenions(), this->_imageSequence.pointsData());

	_ui->loadSequencePushButton->setText("Load");

	// close();
}