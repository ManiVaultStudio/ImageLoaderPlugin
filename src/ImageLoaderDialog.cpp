#include "ImageLoaderDialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStatusBar>

#include "ImageLoaderPlugin.h"
#include "ImageSequenceWidget.h"
#include "ImageStackWidget.h"
#include "MultiPartImageSequenceWidget.h"
#include "ResampleImageSettingsWidget.h"

ImageLoaderDialog::ImageLoaderDialog(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_mainLayout{std::make_unique<QVBoxLayout>()},
	_settingsLayout{std::make_unique<QVBoxLayout>()},
	_typesComboBox{std::make_unique<QComboBox>()},
	_pagesStackedWidget{std::make_unique<StackedWidget>()},
	_imageSequenceWidget{std::make_unique<ImageSequenceWidget>(imageLoaderPlugin)},
	_imageStackWidget{std::make_unique<ImageStackWidget>(imageLoaderPlugin)},
	_multiPartImageSequenceWidget{std::make_unique<MultiPartImageSequenceWidget>(imageLoaderPlugin)},
	_statusBar{std::make_unique<QStatusBar>()}
{
	_mainLayout->setMargin(0);

	setLayout(_mainLayout.get());

	_settingsLayout->setMargin(11);
	_settingsLayout->addWidget(_typesComboBox.get());
	_settingsLayout->addWidget(_pagesStackedWidget.get());
	_settingsLayout->addStretch(1);

	_mainLayout->addLayout(_settingsLayout.get());
	_mainLayout->addWidget(_statusBar.get());

	_pagesStackedWidget->addWidget(_imageSequenceWidget.get());
	_pagesStackedWidget->addWidget(_imageStackWidget.get());
	_pagesStackedWidget->addWidget(_multiPartImageSequenceWidget.get());

	_typesComboBox->addItem("Sequence");
	_typesComboBox->addItem("Stack");
	_typesComboBox->addItem("Multipart");

	_typesComboBox->setItemData(0, "Load in a sequence where each image represents a data point, and the number of dimenions is defined by the number of pixels", Qt::ToolTipRole);
	_typesComboBox->setItemData(1, "Load in a stack of images where each pixel represents a data point, and each layer represents a dimension", Qt::ToolTipRole);
	_typesComboBox->setItemData(1, "Load in one or more multipart TIFF images", Qt::ToolTipRole);

	connect(_typesComboBox.get(), QOverload<int>::of(&QComboBox::currentIndexChanged), _pagesStackedWidget.get(), &QStackedWidget::setCurrentIndex);

	_imageSequenceWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_imageStackWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	setMinimumWidth(480);
	setMinimumHeight(480);

	connect(&_imageSequenceWidget->loader(), &ImageSequenceLoader::message, this, &ImageLoaderDialog::onMessage);
	
	_statusBar->showMessage("Ready");
}

ImageLoaderDialog::~ImageLoaderDialog()
{
}

void ImageLoaderDialog::onMessage(const QString& message)
{
	_statusBar->showMessage(message);
}