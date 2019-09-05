#include "ImageStackWidget.h"

#include "ui_imageStackWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ImageStackWidget::ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageStackWidget>() }
{
	_ui->setupUi(this);

	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageStackWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &ImageStackWidget::onLoadSequence);

	connect(&_imageStacks, &ImageStacks::directoryChanged, this, &ImageStackWidget::onDirectoryChanged);
	connect(&_imageStacks, &ImageStacks::becameDirty, this, &ImageStackWidget::onBecameDirty);
	connect(&_imageStacks, &ImageStacks::beginScan, this, &ImageStackWidget::onBeginScan);
	connect(&_imageStacks, &ImageStacks::endScan, this, &ImageStackWidget::onEndScan);

	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tif";

	_imageStacks.setImageTypes(imageTypes);

	_ui->datasetNameLabel->setEnabled(false);
	_ui->datasetNameLineEdit->setEnabled(false);
	_ui->stacksLabel->setEnabled(false);
	_ui->stacksComboBox->setEnabled(false);
	_ui->loadPushButton->setEnabled(false);

	_ui->resampleImageSettingsWidget->initialize(_imageLoaderPlugin);

	const auto directory = _imageLoaderPlugin->setting("stack/directory", "").toString();

	if (QDir(directory).exists()) {
		_imageStacks.setDirectory(directory);
	}
}

ImageStackWidget::~ImageStackWidget()
{
}

void ImageStackWidget::onBecameDirty()
{
}

void ImageStackWidget::onBeginScan()
{
	_ui->infoLineEdit->setText(QString("Scanning for image stacks..."));
}

void ImageStackWidget::onEndScan()
{
	_ui->stacksComboBox->clear();

	_ui->stacksComboBox->setEnabled(false);

	if (_imageStacks.stacks().size() == 0) {
		_ui->infoLineEdit->setText("No image stacks were found, try changing the directory");
	}
	else {
		_ui->infoLineEdit->setText(QString("Found %1 image stack(s)").arg(_imageStacks.stacks().size()));

		_ui->stacksComboBox->addItems(_imageStacks.stacks().keys());
		
		_ui->datasetNameLabel->setEnabled(true);
		_ui->datasetNameLineEdit->setEnabled(true);
		_ui->stacksLabel->setEnabled(true);
		_ui->stacksComboBox->setEnabled(true);
		_ui->stacksComboBox->setEnabled(true);
	}

	_ui->loadPushButton->setEnabled(true);
}

void ImageStackWidget::onMessage(const QString &message)
{
	_ui->infoLineEdit->setText(message);
}

void ImageStackWidget::onDirectoryChanged(const QString &directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	_imageStacks.start();

	_imageLoaderPlugin->setSetting("stack/directory", directory);
}

void ImageStackWidget::onLoadSequence()
{
	_ui->loadPushButton->setEnabled(false);

	const auto stackName = _ui->stacksComboBox->currentText();

	if (!_imageStacks.stacks().contains(stackName))
		return;

	auto imageStack = _imageStacks.stacks()[_ui->stacksComboBox->currentText()].data();

	connect(imageStack, &ImageStack::beginLoad, this, &ImageStackWidget::onBeginLoad);
	connect(imageStack, &ImageStack::endLoad, this, &ImageStackWidget::onEndLoad);
	connect(imageStack, &ImageStack::message, this, &ImageStackWidget::onMessage);

	imageStack->load();
}

void ImageStackWidget::onPickDirectory()
{
	const auto initialDirectory = _imageLoaderPlugin->setting("stack/directory").toString();
	const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image stack directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_imageStacks.setDirectory(pickedDirectory);

		_imageStacks.start();
	}
}

void ImageStackWidget::onBeginLoad(ImageStack* imageStack)
{
	qDebug() << "Begin loading";

	_ui->loadPushButton->setText("Loading");
}

void ImageStackWidget::onEndLoad(ImageStack* imageStack, std::vector<float>& pointsData)
{
	qDebug() << "End loading";

	disconnect(imageStack, &ImageStack::beginLoad, this, &ImageStackWidget::onBeginLoad);
	disconnect(imageStack, &ImageStack::endLoad, this, &ImageStackWidget::onEndLoad);
	disconnect(imageStack, &ImageStack::message, this, &ImageStackWidget::onMessage);

	_imageLoaderPlugin->addSequence(ImageCollection::Type::Stack, _ui->datasetNameLineEdit->text(), imageStack->size(), imageStack->noImages(), imageStack->noDimensions(), pointsData, imageStack->dimensionNames());

	_ui->loadPushButton->setText("Load");
}
