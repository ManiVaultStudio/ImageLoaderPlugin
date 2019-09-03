#include "ImageLoaderDialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "ImageLoaderPlugin.h"
#include "ImageSequenceWidget.h"
#include "ImageStackWidget.h"
#include "MultiPartImageSequenceWidget.h"

ImageLoaderDialog::ImageLoaderDialog(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_mainLayout{std::make_unique<QVBoxLayout>()},
	_typesComboBox{std::make_unique<QComboBox>()},
	_pagesStackedWidget{std::make_unique<StackedWidget>()},
	_imageSequenceWidget{std::make_unique<ImageSequenceWidget>(imageLoaderPlugin)},
	_imageStackWidget{std::make_unique<ImageStackWidget>(imageLoaderPlugin)},
	_multiPartImageSequenceWidget{std::make_unique<MultiPartImageSequenceWidget>(imageLoaderPlugin)}
{
	setLayout(_mainLayout.get());

	_mainLayout->addWidget(_typesComboBox.get());
	_mainLayout->addWidget(_pagesStackedWidget.get());
	_mainLayout->addStretch(1);

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

	setMinimumWidth(640);
	setMinimumHeight(480);
}

ImageLoaderDialog::~ImageLoaderDialog()
{
}