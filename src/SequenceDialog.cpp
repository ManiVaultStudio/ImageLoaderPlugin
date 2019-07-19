#include "SequenceDialog.h"

#include "ui_SequenceDialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>

#include <FreeImagePlus.h>

#include "PointsPlugin.h"
#include "Set.h"

#include "ImageLoader.h"

SequenceDialog::SequenceDialog(ImageLoader *imageLoader)
	:
	_imageLoaderPlugin(imageLoader),
	_ui{std::make_unique<Ui::SequenceDialog>()},
	_treeModel()
{
	_ui->setupUi(this);

	connect(_ui->directoryPushButton, &QPushButton::clicked, [this]
	{
		const auto _directory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image directory");

		if (!_directory.isNull() || !_directory.isEmpty()) {
			_imageSequence.setDirectory(_directory);
		}
	});

	_ui->imageTypeComboBox->setCurrentText(_imageSequence.imageType());
	_ui->imageWidthSpinBox->setValue(_imageSequence.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_imageSequence.imageSize().height());

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, [this]
	{
		_imageSequence.setImageType(_ui->imageTypeComboBox->currentText());
	});

	connect(_ui->scanPushButton, &QPushButton::clicked, [this]
	{
		_imageSequence.start();
	});

	connect(_ui->imageWidthSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &SequenceDialog::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &SequenceDialog::onImageHeightChanged);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &SequenceDialog::onLoadSequence);
	connect(&_imageSequence, &ImageSequence::directoryChanged, this, &SequenceDialog::onDirectoryChanged);
	connect(&_imageSequence, &ImageSequence::foundImageFile, this, &SequenceDialog::onFoundImageFile);
	connect(&_imageSequence, &ImageSequence::becameDirty, this, &SequenceDialog::onBecameDirty);
	connect(&_imageSequence, &ImageSequence::beginScan, this, &SequenceDialog::onBeginScan);
	connect(&_imageSequence, &ImageSequence::endScan, this, &SequenceDialog::onEndScan);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
}

SequenceDialog::~SequenceDialog()
{
}

void SequenceDialog::onBecameDirty()
{
	_ui->scanPushButton->setEnabled(true);
}

void SequenceDialog::onBeginScan()
{
	_ui->infoLineEdit->setText(QString("Scanning for image files...").arg(_imageSequence.imageFilePaths().size()));
	_ui->scanPushButton->setText("Scanning");
	_ui->scanPushButton->setEnabled(false);
}

void SequenceDialog::onEndScan()
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

void SequenceDialog::onFoundImageFile(const QString &imageFilePath)
{
	_ui->infoLineEdit->setText(QString("Found %1").arg(imageFilePath));
}

void SequenceDialog::onDirectoryChanged(const QString &directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
}

void SequenceDialog::onLoadSequence()
{
}

void SequenceDialog::onImageWidthChanged(int imageWidth)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void SequenceDialog::onImageHeightChanged(int imageHeight)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}