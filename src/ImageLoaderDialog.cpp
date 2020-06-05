#include "ImageLoaderDialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStatusBar>
#include <QProgressBar>

#include "ImageLoaderPlugin.h"

#include "ui_ImageLoaderDialog.h"

ImageLoaderDialog::ImageLoaderDialog(QObject* parent /*= nullptr*/) :
	_ui{ std::make_unique<Ui::ImageLoaderDialog>() },
	_settings("HDPS", "Plugins/ImageLoader/General"),
	_imageLoaderPlugin(nullptr)
{
	_ui->setupUi(this);

	setMinimumWidth(480);
	setMinimumHeight(600);
}

ImageLoaderDialog::~ImageLoaderDialog() = default;

void ImageLoaderDialog::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_ui->commonSettingsWidget->initialize(_imageLoaderPlugin);
}