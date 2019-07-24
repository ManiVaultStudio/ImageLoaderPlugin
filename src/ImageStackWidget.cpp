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
	
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageStackWidget::onPickDirectory);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &ImageStackWidget::onLoadSequence);

	connect(&_imageStack, &ImageStack::directoryChanged, this, &ImageStackWidget::onDirectoryChanged);
	connect(&_imageStack, &ImageStack::message, this, &ImageStackWidget::onMessage);
	connect(&_imageStack, &ImageStack::becameDirty, this, &ImageStackWidget::onBecameDirty);
	connect(&_imageStack, &ImageStack::beginScan, this, &ImageStackWidget::onBeginScan);
	connect(&_imageStack, &ImageStack::endScan, this, &ImageStackWidget::onEndScan);
	connect(&_imageStack, &ImageStack::beginLoad, this, &ImageStackWidget::onBeginLoad);
	connect(&_imageStack, &ImageStack::endLoad, this, &ImageStackWidget::onEndLoad);
}

ImageStackWidget::~ImageStackWidget()
{
}

void ImageStackWidget::onBecameDirty()
{
}

void ImageStackWidget::onBeginScan()
{
	_ui->infoLineEdit->setText(QString("Scanning for image files..."));
}

void ImageStackWidget::onEndScan()
{
	if (_imageStack.imageFilePaths().size() == 0) {
		_ui->infoLineEdit->setText("No images were found, try changing the directory");
	}
	else {
		_ui->infoLineEdit->setText(QString("Found %1 images").arg(_imageStack.imageFilePaths().size()));
	}

	_ui->loadSequencePushButton->setEnabled(true);
}

void ImageStackWidget::onMessage(const QString &message)
{
	_ui->infoLineEdit->setText(message);
}

void ImageStackWidget::onDirectoryChanged(const QString &directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
}

void ImageStackWidget::onLoadSequence()
{
	_imageStack.setRunMode(ImageStack::RunMode::Load);
	_imageStack.start();

	_ui->loadSequencePushButton->setEnabled(false);
}

void ImageStackWidget::onPickDirectory()
{
	const auto _directory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image stack directory");

	if (!_directory.isNull() || !_directory.isEmpty()) {
		_imageStack.setDirectory(_directory);

		_imageStack.setRunMode(ImageStack::RunMode::Scan);
		_imageStack.start();
	}
}

void ImageStackWidget::onBeginLoad()
{
	_ui->loadSequencePushButton->setText("Loading");
}

void ImageStackWidget::onEndLoad()
{
	_imageLoaderPlugin->addSequence(_ui->datasetNameLineEdit->text(), this->_imageStack.noDimenions(), this->_imageStack.pointsData());

	_ui->loadSequencePushButton->setText("Load");
}
