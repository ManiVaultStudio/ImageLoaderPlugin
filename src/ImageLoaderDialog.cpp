#include "ImageLoaderDialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStatusBar>

#include "ImageLoaderPlugin.h"
#include "ImageSequenceWidget.h"
#include "ImageStackWidget.h"
#include "MultiPartImageSequenceWidget.h"

ImageLoaderDialog::ImageLoaderDialog(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_settings("HDPS", "Plugins/ImageLoader/General"),
	_mainLayout(new QVBoxLayout()),
	_settingsLayout(new QVBoxLayout()),
	_typesComboBox(new QComboBox()),
	_pagesStackedWidget(new StackedWidget()),
	_imageSequenceWidget(new ImageSequenceWidget(imageLoaderPlugin)),
	_imageStackWidget(new ImageStackWidget(imageLoaderPlugin)),
	_multiPartImageSequenceWidget(new MultiPartImageSequenceWidget(imageLoaderPlugin)),
	_statusBar(new QStatusBar())
{
	_mainLayout->setMargin(0);

	setLayout(_mainLayout);

	_settingsLayout->setMargin(11);
	_settingsLayout->addWidget(_typesComboBox);
	_settingsLayout->addWidget(_pagesStackedWidget);
	_settingsLayout->addStretch(1);

	_mainLayout->addLayout(_settingsLayout);
	_mainLayout->addWidget(_statusBar);

	_pagesStackedWidget->addWidget(_imageSequenceWidget);
	_pagesStackedWidget->addWidget(_imageStackWidget);
	_pagesStackedWidget->addWidget(_multiPartImageSequenceWidget);

	_typesComboBox->blockSignals(true);

	_typesComboBox->addItem("Sequence");
	_typesComboBox->addItem("Stack");
	_typesComboBox->addItem("Multipart");

	_typesComboBox->setItemData(0, "Load in a sequence where each image represents a data point, and the number of dimensions is defined by the number of pixels", Qt::ToolTipRole);
	_typesComboBox->setItemData(1, "Load in a stack of images where each pixel represents a data point, and each layer represents a dimension", Qt::ToolTipRole);
	_typesComboBox->setItemData(1, "Load in one or more multipart TIFF images", Qt::ToolTipRole);

	_typesComboBox->blockSignals(false);

	connect(_typesComboBox, QOverload<int>::of(&QComboBox::activated), _pagesStackedWidget, &QStackedWidget::setCurrentIndex);

	_imageSequenceWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_imageStackWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	setMinimumWidth(480);
	setMinimumHeight(480);

	connect(_imageSequenceWidget, &ImageSequenceWidget::message, this, &ImageLoaderDialog::onMessage);
	connect(_imageStackWidget, &ImageStackWidget::message, this, &ImageLoaderDialog::onMessage);
	connect(_multiPartImageSequenceWidget, &MultiPartImageSequenceWidget::message, this, &ImageLoaderDialog::onMessage);
	
	const auto CurrentPage = _settings.value("CurrentPage", 0).toInt();

	_typesComboBox->setCurrentIndex(CurrentPage);
	_pagesStackedWidget->setCurrentIndex(CurrentPage);

	_statusBar->showMessage("Ready");
}

ImageLoaderDialog::~ImageLoaderDialog()
{
	_settings.setValue("CurrentPage", _pagesStackedWidget->currentIndex());
}

void ImageLoaderDialog::onMessage(const QString& message)
{
	_statusBar->showMessage(message);
}