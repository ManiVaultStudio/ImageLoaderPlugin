#include "ImageStackWidget.h"

#include "ui_imageStackWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>

#include "ImageLoaderPlugin.h"

ImageStackWidget::ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageStackWidget>() },
	_imageStack()
{
	_ui->setupUi(this);
	
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &ImageStackWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &ImageStackWidget::onLoadSequence);

	connect(&_imageStackScan, &ImageStackScan::directoryChanged, this, &ImageStackWidget::onDirectoryChanged);
	connect(&_imageStackScan, &ImageStackScan::becameDirty, this, &ImageStackWidget::onBecameDirty);
	connect(&_imageStackScan, &ImageStackScan::beginScan, this, &ImageStackWidget::onBeginScan);
	connect(&_imageStackScan, &ImageStackScan::endScan, this, &ImageStackWidget::onEndScan);
	
	auto imageTypes = QStringList();

	imageTypes << "jpg" << "png" << "bmp" << "tif";

	_imageStackScan.setImageTypes(imageTypes);

	_ui->datasetNameLabel->setEnabled(false);
	_ui->datasetNameLineEdit->setEnabled(false);
	_ui->stacksLabel->setEnabled(false);
	_ui->stacksComboBox->setEnabled(false);
	_ui->loadPushButton->setEnabled(false);

	_ui->resampleImageSettingsWidget->initialize(&_imageStack.resampleImageSettings());
}

ImageStackWidget::~ImageStackWidget()
{
}


void ImageStackWidget::scan()
{
	emit _imageStack.beginScan();

	emit _imageStack.message("Scanning for images...");

	_stacks.clear();

	/*
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

	reset();

	scanDir(_directory);

	if (noImages() > 0) {
		emit message(QString("Found %1 images").arg(noImages()));
	}
	else {
		emit message("No images were found, try changing the directory, image type or dimensions");
	}
	*/

	scanDir(_imageStack.directory());

	emit _imageStack.endScan();
}

void ImageStackWidget::scanDir(const QString& directory)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		scanDir(path);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);

	auto nameFilters = QStringList();

	foreach(QString imageType, _imageStack.imageTypes())
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		// qDebug() << imageReader.size();

		const auto size = imageReader.size();

		if (size.width() > 0 && size.height() > 0) {
			const auto sizeString = QString("%1x%2").arg(QString::number(imageReader.size().width()), QString::number(imageReader.size().height()));

			if (!_stacks.contains(sizeString)) {
				_stacks.insert(sizeString, QStringList());
			}

			_stacks[sizeString] << path;
		}

		scanDir(path);
	}
}

void ImageStackWidget::onBecameDirty()
{
}

void ImageStackWidget::onBeginScan()
{
//	_ui->infoLineEdit->setText(QString("Scanning for image stacks..."));
}

void ImageStackWidget::onEndScan()
{
	_ui->stacksComboBox->clear();

	_ui->stacksComboBox->setEnabled(false);

	/*
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
	*/

	_ui->loadPushButton->setEnabled(true);
}

void ImageStackWidget::onDirectoryChanged(const QString &directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	scan();

	_imageStack.setSetting("Directory", directory);
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

void ImageStackWidget::onPickDirectory()
{
	const auto initialDirectory = _imageStack.setting("Directory").toString();
	const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image stack directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_imageStack.setDirectory(pickedDirectory);
	}
}

void ImageStackWidget::onBeginLoad(ImageStack* imageStack)
{
	qDebug() << "Begin loading";

	_ui->loadPushButton->setText("Loading");
}

void ImageStackWidget::onEndLoad(ImageStack* imageStack, std::vector<float>& pointsData)
{
	/*
	qDebug() << "End loading";

	disconnect(imageStack, &ImageStack::beginLoad, this, &ImageStackWidget::onBeginLoad);
	disconnect(imageStack, &ImageStack::endLoad, this, &ImageStackWidget::onEndLoad);
	disconnect(imageStack, &ImageStack::message, this, &ImageStackWidget::onMessage);

	_imageLoaderPlugin->addSequence(ImageCollection::Type::Stack, _ui->datasetNameLineEdit->text(), imageStack->size(), imageStack->noImages(), imageStack->noDimensions(), pointsData, imageStack->dimensionNames());

	_ui->loadPushButton->setText("Load");
	*/
}
