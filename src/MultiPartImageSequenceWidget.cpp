#include "MultiPartImageSequenceWidget.h"

#include "ui_multiPartImageSequenceWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

#include "ImageLoaderPlugin.h"

MultiPartImageSequenceWidget::MultiPartImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::MultiPartImageSequenceWidget>() },
	_scanner(),
	_loader(ImageCollectionType::MultiPartSequence)
{
	_ui->setupUi(this);
	
	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, &_scanner, &MultiPartImageSequenceScanner::setDirectory);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onLoadPushButtonClicked);
	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, &_loader, &ImageCollectionsLoader::setDatasetName);

	connect(&_scanner, &MultiPartImageSequenceScanner::directoryChanged, this, &MultiPartImageSequenceWidget::onDirectoryChanged);
	connect(&_scanner, &MultiPartImageSequenceScanner::beginScan, this, &MultiPartImageSequenceWidget::onBeginScan);
	connect(&_scanner, &MultiPartImageSequenceScanner::endScan, this, &MultiPartImageSequenceWidget::onEndScan);
	
	connect(&_loader, &ImageCollectionsLoader::beginLoad, this, &MultiPartImageSequenceWidget::onBeginLoad);
	connect(&_loader, &ImageCollectionsLoader::endLoad, this, &MultiPartImageSequenceWidget::onEndLoad);
	connect(&_loader, &ImageCollectionsLoader::datasetNameChanged, this, &MultiPartImageSequenceWidget::onDatasetNameChanged);

	connect(&_scanner, &MultiPartImageSequenceScanner::message, this, &MultiPartImageSequenceWidget::message);
	connect(&_loader, &ImageCollectionsLoader::message, this, &MultiPartImageSequenceWidget::message);

	_ui->subsampleImageSettingsWidget->initialize(&_loader.subsampleImageSettings());

	_scanner.loadSettings();
}

MultiPartImageSequenceWidget::~MultiPartImageSequenceWidget()
{
}

void MultiPartImageSequenceWidget::onBecameDirty()
{
//	_ui->scanPushButton->setEnabled(true);
}

void MultiPartImageSequenceWidget::onPickDirectory()
{
	const auto initialDirectory = _scanner.directory();
	const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose multipart image sequence directory", initialDirectory);

	if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
		_scanner.setDirectory(pickedDirectory);
	}
}

void MultiPartImageSequenceWidget::onDirectoryChanged(const QString& directory)
{
	const auto validDirectory = !directory.isEmpty() && QDir(directory).exists();

	if (validDirectory) {
		_ui->directoryLineEdit->setText(directory);
		_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
	}

	_ui->datasetNameLabel->setEnabled(validDirectory);
	_ui->datasetNameLineEdit->setEnabled(validDirectory);

	_scanner.scan();
}

void MultiPartImageSequenceWidget::onLoadPushButtonClicked()
{
	_loader.load(_scanner.scanned());

	_ui->loadPushButton->setEnabled(false);
}

void MultiPartImageSequenceWidget::onDatasetNameChanged(const QString& dataSetName)
{
	_ui->loadPushButton->setEnabled(!dataSetName.isEmpty() && _scanner.scanned().loadable());
}

void MultiPartImageSequenceWidget::onBeginScan()
{
}

void MultiPartImageSequenceWidget::onEndScan(const ImageCollections& scannedImageCollections)
{
	const auto loadable = _scanner.scanned().loadable();

	_ui->imagesLabel->setEnabled(loadable);
	_ui->imagesListWidget->setEnabled(loadable);
	_ui->datasetNameLabel->setEnabled(loadable);
	_ui->datasetNameLineEdit->setEnabled(loadable);
	_ui->loadPushButton->setEnabled(loadable);

	_ui->imagesListWidget->clear();

	/*
	foreach(QString imageFilePath, imageFilePaths)
	{
		const auto imageFileName = QFileInfo(imageFilePath).fileName();

		QListWidgetItem* fileItem = new QListWidgetItem;
		
		fileItem->setFlags(fileItem->flags() | Qt::ItemIsUserCheckable);
		fileItem->setCheckState(imageFileName.contains("mask") ? Qt::Unchecked : Qt::Checked);
		fileItem->setData(Qt::UserRole, QVariant(imageFilePath));
		fileItem->setText(imageFileName);
		fileItem->setToolTip(imageFilePath);

		_ui->imagesListWidget->addItem(fileItem);
	}
	*/
}

void MultiPartImageSequenceWidget::onBeginLoad()
{
	qDebug() << "onBeginLoad";
}

void MultiPartImageSequenceWidget::onEndLoad(ImageDataSet& imageDataSet)
{
	qDebug() << "onEndLoad";

	_ui->loadPushButton->setEnabled(false);
	_ui->loadPushButton->setText("Load");

	_imageLoaderPlugin->addImageDataSet(imageDataSet);
}