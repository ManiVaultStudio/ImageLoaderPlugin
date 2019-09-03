#include "MultiPartImageSequenceWidget.h"

#include "ui_multiPartImageSequenceWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

#include "ImageLoaderPlugin.h"

MultiPartImageSequenceWidget::MultiPartImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::MultiPartImageSequenceWidget>() }
{
	_ui->setupUi(this);
	/*
	const auto width	= _imageLoaderPlugin->_settings.value("stack/width", QVariant(28)).toInt();
	const auto height	= _imageLoaderPlugin->_settings.value("stack/height", QVariant(28)).toInt();

	_imageSequence.setImageSize(QSize(width, height));

	_ui->imageTypeComboBox->setCurrentText(_imageSequence.imageType());
	_ui->imageWidthSpinBox->setValue(_imageSequence.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_imageSequence.imageSize().height());

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, this, &MultiPartImageSequenceWidget::onImageTypeChanged);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onPickDirectory);
	connect(_ui->scanPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onScan);
	connect(_ui->imageWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MultiPartImageSequenceWidget::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MultiPartImageSequenceWidget::onImageHeightChanged);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onLoadSequence);
	connect(&_imageSequence, &ImageSequence::directoryChanged, this, &MultiPartImageSequenceWidget::onDirectoryChanged);
	connect(&_imageSequence, &ImageSequence::message, this, &MultiPartImageSequenceWidget::onMessage);
	connect(&_imageSequence, &ImageSequence::becameDirty, this, &MultiPartImageSequenceWidget::onBecameDirty);
	connect(&_imageSequence, &ImageSequence::beginScan, this, &MultiPartImageSequenceWidget::onBeginScan);
	connect(&_imageSequence, &ImageSequence::endScan, this, &MultiPartImageSequenceWidget::onEndScan);
	connect(&_imageSequence, &ImageSequence::beginLoad, this, &MultiPartImageSequenceWidget::onBeginLoad);
	connect(&_imageSequence, &ImageSequence::endLoad, this, &MultiPartImageSequenceWidget::onEndLoad);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
	_ui->imageTypeComboBox->addItem("tif");

	const auto directory = _imageLoaderPlugin->_settings.value("sequence/directory", "").toString();

	if (QDir(directory).exists()) {
		_imageSequence.setDirectory(directory);
	}
	*/
}

MultiPartImageSequenceWidget::~MultiPartImageSequenceWidget()
{
}

/*
void MultiPartImageSequenceWidget::onBecameDirty()
{
	_ui->scanPushButton->setEnabled(true);
}

void MultiPartImageSequenceWidget::onBeginScan()
{
	_ui->infoLineEdit->setText(QString("Scanning for image files..."));
	_ui->scanPushButton->setText("Scanning");
	_ui->scanPushButton->setEnabled(false);
}

void MultiPartImageSequenceWidget::onEndScan()
{
	const auto noImages = _imageSequence.imageFilePaths().size();

	if (noImages <= 0) {
		_ui->infoLineEdit->setText("No images were found, try changing the directory, image type or dimensions");
	}
	else {
		_ui->infoLineEdit->setText(QString("Found %1 images").arg(noImages));
		_ui->loadSequencePushButton->setEnabled(true);
	}
	
	_ui->scanPushButton->setText("Scan");
}

void MultiPartImageSequenceWidget::onMessage(const QString &message)
{
	// qDebug() << message;

	_ui->infoLineEdit->setText(message);
}
*/
void MultiPartImageSequenceWidget::onDirectoryChanged(const QString& directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	//_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
	//_imageSequence.start();

	_imageLoaderPlugin->_settings.setValue("multipart/directory", directory);
}
/*
void MultiPartImageSequenceWidget::onLoadSequence()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Load);
	_imageSequence.start();

	_ui->loadSequencePushButton->setEnabled(false);
}

void MultiPartImageSequenceWidget::onImageWidthChanged(int imageWidth)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));

	_imageLoaderPlugin->_settings.setValue("stack/width", imageWidth);
}

void MultiPartImageSequenceWidget::onImageHeightChanged(int imageHeight)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));

	_imageLoaderPlugin->_settings.setValue("stack/height", imageHeight);
}

void MultiPartImageSequenceWidget::onScan()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
	_imageSequence.start();
}
*/
void MultiPartImageSequenceWidget::onPickDirectory()
{
	const auto initialDirectory = _imageLoaderPlugin->_settings.value("multipart/directory").toString();
	const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose multipart image sequence directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		//_imageSequence.setDirectory(pickedDirectory);

		//_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
		//_imageSequence.start();
	}
}
/*
void MultiPartImageSequenceWidget::onImageTypeChanged(const QString & imageType)
{
	_imageSequence.setImageType(_ui->imageTypeComboBox->currentText());
}

void MultiPartImageSequenceWidget::onBeginLoad()
{
	_ui->loadSequencePushButton->setText("Loading");
}

void MultiPartImageSequenceWidget::onEndLoad()
{
	_imageLoaderPlugin->addSequence(ImageLoaderPlugin::ImageCollectionType::Sequence, _ui->datasetNameLineEdit->text(), _imageSequence.imageSize(), _imageSequence.noImages(), _imageSequence.noDimensions(), _imageSequence.pointsData(), _imageSequence.dimensionNames());

	_ui->loadSequencePushButton->setText("Load");

	// close();
}
*/