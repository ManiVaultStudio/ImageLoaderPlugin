#include "CommonSettingsWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_CommonSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

CommonSettingsWidget::CommonSettingsWidget(QWidget* parent) :
	QWidget(parent),
	_imageLoaderPlugin(nullptr),
	_ui{ std::make_unique<Ui::CommonSettingsWidget>() },
	_scanner(),
	_loader(nullptr, ImageData::Type::Sequence)
{
	_ui->setupUi(this);
}

CommonSettingsWidget::~CommonSettingsWidget() = default;

void CommonSettingsWidget::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_scanner.setImageLoaderPlugin(imageLoaderPlugin);

	_ui->sequencesTreeView->setModel(&_imageLoaderPlugin->imageCollectionsModel());
	_ui->imagesTreeView->setModel(&_imageLoaderPlugin->imagesModel());

	_scanner.scan();

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString directory) {
		_scanner.setDirectory(directory);
	});

	connect(&_scanner, &ImageScanner::directoryChanged, [this](const QString& directory) {
		_ui->directoryLineEdit->blockSignals(true);
		_ui->directoryLineEdit->setText(directory);
		_ui->directoryLineEdit->blockSignals(false);

		_scanner.scan();
	});

	connect(_ui->directoryPushButton, &QPushButton::clicked, [this]() {
		const auto initialDirectory	= _scanner.directory();
		const auto pickedDirectory	= QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image sequence directory", initialDirectory);

		if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
			_scanner.setDirectory(pickedDirectory);
		}
	});

	/*
	connect(&_loader, &ImageLoader::settingsChanged, [this]() {
		const auto enableLoad = _scanner.scanned()->loadable();
	});
	*/

	_scanner.loadSettings();
}