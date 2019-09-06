#include "ImageStackWidget.h"

#include "ui_imageStackWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

ImageStackWidget::ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageStackWidget>() },
	_scanner(),
	_loader()
{
	_ui->setupUi(this);
	
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageStackWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &ImageStackWidget::onLoadSequence);

	connect(&_scanner, &ImageStackScanner::directoryChanged, this, &ImageStackWidget::onDirectoryChanged);
	connect(&_scanner, &ImageStackScanner::beginScan, this, &ImageStackWidget::onBeginScan);
	connect(&_scanner, &ImageStackScanner::endScan, this, &ImageStackWidget::onEndScan);

	_ui->resampleImageSettingsWidget->initialize(&_loader.resampleImageSettings());

	_scanner.setDirectory(_scanner.directory());
}

void ImageStackWidget::onPickDirectory()
{
	const auto initialDirectory = _loader.setting("Directory").toString();
	const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image stack directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_scanner.setDirectory(pickedDirectory);
	}
}

void ImageStackWidget::onDirectoryChanged(const QString& directory)
{
	qDebug() << "Image stack scan directory changed to" << directory;

	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	_scanner.scan();
}

void ImageStackWidget::onLoadSequence()
{
	/*
	_ui->loadPushButton->setEnabled(false);

	const auto stackName = _ui->stacksComboBox->currentText();

	if (!_imageStacks.stacks().contains(stackName))
		return;

	auto imageStack = _imageStacks.stacks()[_ui->stacksComboBox->currentText()].data();

	connect(imageStack, &ImageStack::beginLoad, this, &ImageStackWidget::onBeginLoad);
	connect(imageStack, &ImageStack::endLoad, this, &ImageStackWidget::onEndLoad);
	connect(imageStack, &ImageStack::message, this, &ImageStackWidget::onMessage);

	imageStack->load();
	*/
}

void ImageStackWidget::onBeginScan()
{
	qDebug() << "Image stack scan started";

	//	_ui->infoLineEdit->setText(QString("Scanning for image stacks..."));
}

void ImageStackWidget::onEndScan(QMap<QString, QStringList>& imageStacks)
{
	qDebug() << "Image stack scan ended";

	_ui->stacksComboBox->clear();

	const auto noStacks		= imageStacks.size();
	const auto hasStacks	= noStacks > 0;

	_ui->datasetNameLabel->setEnabled(hasStacks);
	_ui->datasetNameLineEdit->setEnabled(hasStacks);
	_ui->stacksLabel->setEnabled(hasStacks);
	_ui->stacksComboBox->setEnabled(hasStacks);
	_ui->loadPushButton->setEnabled(hasStacks);

	if (hasStacks) {
		_ui->stacksComboBox->addItems(imageStacks.keys());
	}
	
	_ui->loadPushButton->setEnabled(true);
}

void ImageStackWidget::onBeginLoad()
{
	qDebug() << "Image stack loading";

	_ui->loadPushButton->setText("Loading");
}

void ImageStackWidget::onEndLoad(FloatVector& pointsData)
{
	qDebug() << "Image stack loaded";

	//_imageLoaderPlugin->addSequence(ImageCollectionType::Stack, _ui->datasetNameLineEdit->text(), imageStack->size(), imageStack->noImages(), imageStack->noDimensions(), pointsData, imageStack->dimensionNames());

	/*
	qDebug() << "End loading";

	disconnect(imageStack, &ImageStack::beginLoad, this, &ImageStackWidget::onBeginLoad);
	disconnect(imageStack, &ImageStack::endLoad, this, &ImageStackWidget::onEndLoad);
	disconnect(imageStack, &ImageStack::message, this, &ImageStackWidget::onMessage);

	_imageLoaderPlugin->addSequence(ImageCollectionType::Stack, _ui->datasetNameLineEdit->text(), imageStack->size(), imageStack->noImages(), imageStack->noDimensions(), pointsData, imageStack->dimensionNames());

	_ui->loadPushButton->setText("Load");
	*/
}
