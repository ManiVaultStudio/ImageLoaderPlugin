#include "ImageStackWidget.h"

#include "ui_ImageStackWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoader.h"

ImageStackWidget::ImageStackWidget(ImageLoader *imageLoader) :
	_imageLoaderPlugin(imageLoader),
	_ui{ std::make_unique<Ui::ImageStackWidget>() }
{
	_ui->setupUi(this);
	/*
	_ui->imageTypeComboBox->setCurrentText(_imageSequence.imageType());
	_ui->imageWidthSpinBox->setValue(_imageSequence.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_imageSequence.imageSize().height());

	connect(_ui->loaderTypeComboBox, SIGNAL(activated(int)), _ui->loaderPagesStackedWidget, SLOT(setCurrentIndex(int)));
	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, this, &SequenceDialog::onImageTypeChanged);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &SequenceDialog::onPickDirectory);
	connect(_ui->scanPushButton, &QPushButton::clicked, this, &SequenceDialog::onScan);
	connect(_ui->imageWidthSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &SequenceDialog::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &SequenceDialog::onImageHeightChanged);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &SequenceDialog::onLoadSequence);
	connect(&_imageSequence, &ImageSequence::directoryChanged, this, &SequenceDialog::onDirectoryChanged);
	connect(&_imageSequence, &ImageSequence::message, this, &SequenceDialog::onMessage);
	connect(&_imageSequence, &ImageSequence::becameDirty, this, &SequenceDialog::onBecameDirty);
	connect(&_imageSequence, &ImageSequence::beginScan, this, &SequenceDialog::onBeginScan);
	connect(&_imageSequence, &ImageSequence::endScan, this, &SequenceDialog::onEndScan);
	connect(&_imageSequence, &ImageSequence::beginLoad, this, &SequenceDialog::onBeginLoad);
	connect(&_imageSequence, &ImageSequence::endLoad, this, &SequenceDialog::onEndLoad);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");

	// onPickDirectory();
	*/
}

ImageStackWidget::~ImageStackWidget()
{
}

/*
void ImageStackWidget::onBecameDirty()
{
	_ui->scanPushButton->setEnabled(true);
}

void ImageStackWidget::onBeginScan()
{
	_ui->infoLineEdit->setText(QString("Scanning for image files..."));
	_ui->scanPushButton->setText("Scanning");
	_ui->scanPushButton->setEnabled(false);
}

void ImageStackWidget::onEndScan()
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

void ImageStackWidget::onMessage(const QString &message)
{
	// qDebug() << message;

	_ui->infoLineEdit->setText(message);
}

void ImageStackWidget::onDirectoryChanged(const QString &directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
}

void ImageStackWidget::onLoadSequence()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Load);
	_imageSequence.start();

	_ui->loadSequencePushButton->setEnabled(false);
}

void ImageStackWidget::onImageWidthChanged(int imageWidth)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageStackWidget::onImageHeightChanged(int imageHeight)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageStackWidget::onScan()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
	_imageSequence.start();
}

void ImageStackWidget::onPickDirectory()
{
	const auto _directory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory");

	if (!_directory.isNull() || !_directory.isEmpty()) {
		_imageSequence.setDirectory(_directory);

		_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
		_imageSequence.start();
	}
}

void ImageStackWidget::onImageTypeChanged(const QString & imageType)
{
	_imageSequence.setImageType(_ui->imageTypeComboBox->currentText());
}

void ImageStackWidget::onBeginLoad()
{
	_ui->loadSequencePushButton->setText("Loading");
}

void ImageStackWidget::onEndLoad()
{
	_imageLoaderPlugin->addSequence(_ui->datasetNameLineEdit->text(), this->_imageSequence.noDimenions(), this->_imageSequence.pointsData());

	_ui->loadSequencePushButton->setText("Load");

	// close();
}
*/