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
	
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onPickDirectory);
	connect(_ui->loadPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onLoadPushButtonClicked);
	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, &_loader, &ImageCollectionsLoader::setDatasetName);

	connect(&_scanner, &MultiPartImageSequenceScanner::directoryChanged, this, &MultiPartImageSequenceWidget::onDirectoryChanged);
	connect(&_scanner, &MultiPartImageSequenceScanner::beginScan, this, &MultiPartImageSequenceWidget::onBeginScan);
	connect(&_scanner, &MultiPartImageSequenceScanner::endScan, this, &MultiPartImageSequenceWidget::onEndScan);
	connect(&_scanner, &MultiPartImageSequenceScanner::message, this, &MultiPartImageSequenceWidget::message);
	
	connect(&_loader, &ImageCollectionsLoader::message, this, &MultiPartImageSequenceWidget::message);
	connect(&_loader, &ImageCollectionsLoader::datasetNameChanged, this, &MultiPartImageSequenceWidget::onDatasetNameChanged);

	_ui->subsampleImageSettingsWidget->initialize(&_loader.subsampleImageSettings());

	_scanner.setDirectory(_scanner.directory());
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
	//qDebug() << _ui->imagesListWidget->item
}

void MultiPartImageSequenceWidget::onDatasetNameChanged(const QString& dataSetName)
{
	_ui->loadPushButton->setEnabled(!dataSetName.isEmpty() && _scanner.scanned().loadable());
}

void MultiPartImageSequenceWidget::onBeginScan()
{
	qDebug() << "Scanning started";
}

void MultiPartImageSequenceWidget::onEndScan(const ImageCollections& scannedImageCollections)
{
	/*
	qDebug() << "Scanning ended";

	_ui->imagesListWidget->clear();

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
}

void MultiPartImageSequenceWidget::onEndLoad(ImageDataSet& imageDataSet)
{
	//_imageLoaderPlugin->addSequence(ImageCollectionType::Sequence, _ui->datasetNameLineEdit->text(), _imageSequence.imageSize(), _imageSequence.noImages(), _imageSequence.noDimensions(), _imageSequence.pointsData(), _imageSequence.dimensionNames());
}