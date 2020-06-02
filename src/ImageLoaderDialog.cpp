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

	const auto currentPage = _settings.value("CurrentPage", 0).toInt();

	_ui->settingsStackedWidget->setCurrentIndex(currentPage);

	QObject::connect(_ui->sequencePushButton, &QPushButton::clicked, [this]() {
		_ui->settingsStackedWidget->setCurrentIndex(0);
	});

	QObject::connect(_ui->stackPushButton, &QPushButton::clicked, [this]() {
		_ui->settingsStackedWidget->setCurrentIndex(1);
	});

	_ui->sequencePushButton->setChecked(currentPage == 0);
	_ui->stackPushButton->setChecked(currentPage == 1);

	QObject::connect(_ui->settingsStackedWidget, &QStackedWidget::currentChanged, [this](int currentPage) {
		_ui->sequencePushButton->setChecked(currentPage == 0);
		_ui->stackPushButton->setChecked(currentPage == 1);
	});
}

ImageLoaderDialog::~ImageLoaderDialog()
{
	_settings.setValue("CurrentPage", _ui->settingsStackedWidget->currentIndex());
}

void ImageLoaderDialog::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_ui->sequenceSettingsWidget->initialize(_imageLoaderPlugin);
}