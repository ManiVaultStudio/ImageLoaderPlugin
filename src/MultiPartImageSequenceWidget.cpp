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
	
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onPickDirectory);
	//connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onLoadSequence);
	connect(&_multipartImageSequence, &MultipartImageSequence::directoryChanged, this, &MultiPartImageSequenceWidget::onDirectoryChanged);
	/*
	connect(&_imageSequence, &ImageSequence::message, this, &MultiPartImageSequenceWidget::onMessage);
	connect(&_imageSequence, &ImageSequence::becameDirty, this, &MultiPartImageSequenceWidget::onBecameDirty);
	connect(&_imageSequence, &ImageSequence::endScan, this, &MultiPartImageSequenceWidget::onEndScan);
	connect(&_imageSequence, &ImageSequence::beginLoad, this, &MultiPartImageSequenceWidget::onBeginLoad);
	connect(&_imageSequence, &ImageSequence::endLoad, this, &MultiPartImageSequenceWidget::onEndLoad);
	*/

	const auto directory = _imageLoaderPlugin->setting("multipart/directory", "").toString();

	if (QDir(directory).exists()) {
		_multipartImageSequence.setDirectory(directory);
	}

	const auto subsamplingRatio = _imageLoaderPlugin->setting("multipart/subsampling/ratio", "").toDouble();


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

	_imageLoaderPlugin->setSetting("multipart/directory", directory);
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
	const auto initialDirectory = _imageLoaderPlugin->setting("multipart/directory").toString();
	const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose multipart image sequence directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_multipartImageSequence.setDirectory(pickedDirectory);

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