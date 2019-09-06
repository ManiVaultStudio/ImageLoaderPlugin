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
	_loader()
{
	_ui->setupUi(this);
	
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &MultiPartImageSequenceWidget::onPickDirectory);

	connect(&_scanner, &MultiPartImageSequenceScanner::directoryChanged, this, &MultiPartImageSequenceWidget::onDirectoryChanged);
	connect(&_scanner, &MultiPartImageSequenceScanner::beginScan, this, &MultiPartImageSequenceWidget::onBeginScan);
	connect(&_scanner, &MultiPartImageSequenceScanner::endScan, this, &MultiPartImageSequenceWidget::onEndScan);

	_ui->resampleImageSettingsWidget->initialize(&_loader.resampleImageSettings());

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
	//_ui->directoryLineEdit->setText(directory);
	//_ui->datasetNameLineEdit->setText(QDir(directory).dirName());

	_scanner.scan();
}

void MultiPartImageSequenceWidget::onBeginScan()
{
	qDebug() << "Multipart image sequence scan started";
}

void MultiPartImageSequenceWidget::onEndScan()
{
	qDebug() << "Multipart image sequence scan ended";

	/*
	const auto noImages = _imageSequence.imageFilePaths().size();

	if (noImages <= 0) {
		_ui->infoLineEdit->setText("No images were found, try changing the directory, image type or dimensions");
	}
	else {
		_ui->infoLineEdit->setText(QString("Found %1 images").arg(noImages));
		_ui->loadSequencePushButton->setEnabled(true);
	}

	_ui->scanPushButton->setText("Scan");
	*/
}

void MultiPartImageSequenceWidget::onBeginLoad()
{
	_ui->loadSequencePushButton->setText("Loading");
}

void MultiPartImageSequenceWidget::onEndLoad()
{
	//_imageLoaderPlugin->addSequence(ImageCollectionType::Sequence, _ui->datasetNameLineEdit->text(), _imageSequence.imageSize(), _imageSequence.noImages(), _imageSequence.noDimensions(), _imageSequence.pointsData(), _imageSequence.dimensionNames());

	_ui->loadSequencePushButton->setText("Load");
}