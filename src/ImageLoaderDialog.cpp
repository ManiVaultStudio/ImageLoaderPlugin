#include "ImageLoaderDialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QComboBox>

#include "ImageLoader.h"
#include "ImageSequenceWidget.h"
#include "ImageStackWidget.h"

ImageLoaderDialog::ImageLoaderDialog(ImageLoader *imageLoader) :
	_imageLoaderPlugin(imageLoader),
	_mainLayout{std::make_unique<QVBoxLayout>()},
	_typesComboBox{std::make_unique<QComboBox>()},
	_pagesStackedWidget{std::make_unique<StackedWidget>()},
	_imageSequenceWidget{std::make_unique<ImageSequenceWidget>(imageLoader)},
	_imageStackWidget{std::make_unique<ImageStackWidget>(imageLoader)}
{
	setLayout(_mainLayout.get());

	_mainLayout->addWidget(_typesComboBox.get());
	_mainLayout->addWidget(_pagesStackedWidget.get());
	//_mainLayout->addWidget(QSpacerItem(nullptr, .get());
	_mainLayout->addStretch(1);

	_pagesStackedWidget->addWidget(_imageSequenceWidget.get());
	_pagesStackedWidget->addWidget(_imageStackWidget.get());

	_typesComboBox->addItem("Sequence of images");
	_typesComboBox->addItem("Stack of images");

	_typesComboBox->setItemData(0, "Load in a sequence where each image represents a data point, and the number of dimenions is defined by the number of pixels", Qt::ToolTipRole);
	_typesComboBox->setItemData(1, "Load in a stack of images where each pixel represents a data point, and each layer represents a dimension", Qt::ToolTipRole);

	connect(_typesComboBox.get(), SIGNAL(activated(int)), _pagesStackedWidget.get(), SLOT(setCurrentIndex(int)));
	connect(_pagesStackedWidget.get(), SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged()));

	//_pagesStackedWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_imageSequenceWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_imageStackWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	//setMinimumHeight(0);

	//_mainLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
	//setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	//setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

	/*
	_ui->imageTypeComboBox->setCurrentText(_imageSequence.imageType());
	_ui->imageWidthSpinBox->setValue(_imageSequence.imageSize().width());
	_ui->imageHeightSpinBox->setValue(_imageSequence.imageSize().height());

	
	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, this, &SequenceDialog::onImageTypeChanged);
	connect(_ui->directoryPushButton, &QPushButton::clicked, this, &SequenceDialog::onPickDirectory);
	connect(_ui->scanPushButton, &QPushButton::clicked, this, &SequenceDialog::onScan);
	connect(_ui->imageWidthSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &SequenceDialog::onImageWidthChanged);
	connect(_ui->imageHeightSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &SequenceDialog::onImageHeightChanged);
	connect(_ui->loadSequencePushButton, &QPushButton::clicked, this, &SequenceDialog::onLoadSequence);
	connect(&_imageSequence, &ImageSequence::directoryChanged, this, &SequenceDialog::onDirectoryChanged);
	connect(&_imageSequence, &ImageSequence::message, this, &SequenceDialog::onMessage);
	connect(&_imageSequence, &ImageSequence::becameDirty, this, &SequenceDialog::onBecameDirty);
	connect(&_imageSequence, &ImageSequence::beginScan, this, &SequenceDialog::onBeginScan);
	connect(&_imageSequence, &ImageSequence::endScan, this, &SequenceDialog::onEndScan);
	connect(&_imageSequence, &ImageSequence::beginLoad, this, &SequenceDialog::onBeginLoad);
	connect(&_imageSequence, &ImageSequence::endLoad, this, &SequenceDialog::onEndLoad);

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");

	// onPickDirectory();
	*/
}

ImageLoaderDialog::~ImageLoaderDialog()
{
}

void ImageLoaderDialog::onTypeChanged()
{
	// _pagesStackedWidget->adjustSize();

	//adjustSize();
}

/*
void ImageLoaderDialog::onBecameDirty()
{
	_ui->scanPushButton->setEnabled(true);
}

void ImageLoaderDialog::onBeginScan()
{
	_ui->infoLineEdit->setText(QString("Scanning for image files..."));
	_ui->scanPushButton->setText("Scanning");
	_ui->scanPushButton->setEnabled(false);
}

void ImageLoaderDialog::onEndScan()
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

void ImageLoaderDialog::onMessage(const QString &message)
{
	// qDebug() << message;

	_ui->infoLineEdit->setText(message);
}

void ImageLoaderDialog::onDirectoryChanged(const QString &directory)
{
	_ui->directoryLineEdit->setText(directory);
	_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
}

void ImageLoaderDialog::onLoadSequence()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Load);
	_imageSequence.start();

	_ui->loadSequencePushButton->setEnabled(false);
}

void ImageLoaderDialog::onImageWidthChanged(int imageWidth)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageLoaderDialog::onImageHeightChanged(int imageHeight)
{
	_imageSequence.setImageSize(QSize(_ui->imageWidthSpinBox->value(), _ui->imageHeightSpinBox->value()));
}

void ImageLoaderDialog::onScan()
{
	_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
	_imageSequence.start();
}

void ImageLoaderDialog::onPickDirectory()
{
	const auto _directory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory");

	if (!_directory.isNull() || !_directory.isEmpty()) {
		_imageSequence.setDirectory(_directory);

		_imageSequence.setRunMode(ImageSequence::RunMode::Scan);
		_imageSequence.start();
	}
}

void ImageLoaderDialog::onImageTypeChanged(const QString & imageType)
{
	_imageSequence.setImageType(_ui->imageTypeComboBox->currentText());
}

void ImageLoaderDialog::onBeginLoad()
{
	_ui->loadSequencePushButton->setText("Loading");
}

void ImageLoaderDialog::onEndLoad()
{
	_imageLoaderPlugin->addSequence(_ui->datasetNameLineEdit->text(), this->_imageSequence.noDimenions(), this->_imageSequence.pointsData());

	_ui->loadSequencePushButton->setText("Load");

	// close();
}
*/

inline QSize StackedWidget::sizeHint() const
{
	qDebug() << "sizeHint" << currentWidget()->sizeHint();
	return currentWidget()->sizeHint();
}

inline QSize StackedWidget::minimumSizeHint() const
{
	qDebug() << "minimumSizeHint" << currentWidget()->minimumSizeHint();
	return currentWidget()->minimumSizeHint();
}
