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
		_imageSequence.scan();
	});

	connect(_ui->loadSequencePushButton, &QPushButton::clicked, [this]
	{
		// _imageLoaderPlugin->AddSequence(_ui->datasetNameLineEdit->text(), _imageFilePaths);
	});

	connect(_ui->imageWidthSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val) {
		_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
	});

	connect(_ui->imageHeightSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val) {
		_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
	});

	connect(&_imageSequence, static_cast<void (ImageSequence::*)(const QString &)>(&ImageSequence::directoryChanged), this, [this](const QString &directory) {
		_ui->directoryLineEdit->setText(directory);
		_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
	});

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
}

SequenceDialog::~SequenceDialog()
{
}

void SequenceDialog::Scan() {
	/*
	qDebug() << "Scan";

	const auto imageSize = QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value());

	ScanDirectoryForImages(_directory, _ui->imageTypeComboBox->currentText(), imageSize);
	*/
}

void SequenceDialog::ScanDirectoryForImages(const QString& rootPath, const QString &imageType, const QSize &imageSize)
{
	/*
	_rootPath	= rootPath;
	_imageType	= imageType;
	_imageSize	= imageSize;

	// _ui->infoLineEdit->setText("Scanning for " + _imageType + " images at " + imageSize.width() + "x" + imageSize.height() + " ...");
	
	qDebug() << "Scanning: " << rootPath;

	_imageFilePaths.clear();

	ScanDir(_rootPath);

	_ui->infoLineEdit->setText("Found " + QString::number(_imageFilePaths.size()) + " images");

	qDebug() << "Found " << _imageFilePaths.size() << " images";
	*/
}

void SequenceDialog::ScanDir(const QString &directory)
{
	/*
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		ScanDir(path);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(QStringList() << "*." + _imageType);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		if (imageReader.size() == _imageSize) {
			// qDebug() << "Found image: " << fileList.at(i);

			_imageFilePaths.append(path);

			ScanDir(path);
		}
	}
	*/
}
