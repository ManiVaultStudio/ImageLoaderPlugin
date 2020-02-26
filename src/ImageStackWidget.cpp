#include "ImageStackWidget.h"

#include "ui_ImageStackWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QEvent>

#include "ImageLoaderPlugin.h"

ImageStackWidget::ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ui{ std::make_unique<Ui::ImageStackWidget>() },
	_scanner(),
	_loader(imageLoaderPlugin, ImageCollectionType::Stack)
{
	_ui->setupUi(this);

	connect(_ui->directoryLineEdit, &QLineEdit::textChanged, [this](QString directory) {
		_scanner.setDirectory(directory);
	});

	connect(&_scanner, &ImageStackScanner::directoryChanged, [this](const QString& directory) {
		_ui->directoryLineEdit->blockSignals(true);
		_ui->directoryLineEdit->setText(directory);
		_ui->directoryLineEdit->blockSignals(false);
		_ui->datasetNameLineEdit->setEnabled(!directory.isEmpty());
		_ui->datasetNameLineEdit->setText(QDir(directory).dirName());
	});

	connect(_ui->pickDirectoryPushButton, &QPushButton::clicked, [this]() {
		const auto initialDirectory = _loader.setting("Directory").toString();
		const auto pickedDirectory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image stack directory", initialDirectory);

		if (!pickedDirectory.isNull() || !pickedDirectory.isEmpty()) {
			_scanner.setDirectory(pickedDirectory);
		}
	});

	connect(&_loader, &ImageLoader::datasetNameChanged, [this](const QString& datasetName) {
		_ui->datasetNameLineEdit->blockSignals(true);
		_ui->datasetNameLineEdit->setText(datasetName);
		_ui->datasetNameLineEdit->blockSignals(false);
	});

	connect(_ui->datasetNameLineEdit, &QLineEdit::textChanged, [this](const QString& text) {
		_loader.setDatasetName(text);
	});
	
	connect(_ui->loadPushButton, &QPushButton::clicked, [this]() {
		foreach(QString key, _scanner.scanned()->map().keys()) {
			if (key != _ui->stacksComboBox->currentData().toString()) {
				_scanner.scanned()->map().remove(key);
			}
		}

		_loader.load(_scanner.scanned());

		_ui->loadPushButton->setEnabled(false);
	});

	connect(&_scanner, &ImageStackScanner::settingsChanged, [this]() {
		_scanner.scan();
	});

	connect(&_loader, &ImageLoader::settingsChanged, [this]() {
		const auto enableLoad = _scanner.scanned()->loadable();
		_ui->loadPushButton->setEnabled(enableLoad);
	});

	connect(&_scanner, &ImageStackScanner::endScan, [this](std::shared_ptr<Scanned> scanned) {
		const auto loadable = scanned->loadable();

		_ui->datasetNameLabel->setEnabled(loadable);
		_ui->datasetNameLineEdit->setEnabled(loadable);
		_ui->stacksLabel->setEnabled(loadable);
		_ui->stacksComboBox->setEnabled(loadable);
		_ui->loadPushButton->setEnabled(loadable);

		_ui->stacksComboBox->clear();

		auto items = QStringList();

		const auto stacks = scanned->map();

		for (const auto& key : stacks.keys()) {
			const auto title = QString("%1 (%2 dimensions)").arg(key, QString::number(stacks[key].noImages()));

			_ui->stacksComboBox->addItem(title, QVariant::fromValue(key));
		}

		_ui->stacksComboBox->addItems(items);
	});
	
	connect(&_loader, &ImageLoader::beginLoad, this, [this]() {
		_ui->loadPushButton->setText("Loading...");
	}, Qt::QueuedConnection);

	connect(&_loader, &ImageLoader::endLoad, this, [this](std::shared_ptr<Payload> payload) {
		_ui->loadPushButton->setEnabled(false);
		_ui->loadPushButton->setText("Load");
	}, Qt::QueuedConnection);

	_ui->subsampleSettingsWidget->initialize(&_loader.subsampleImageSettings());
	_ui->colorSettingsWidget->initialize(&_loader.colorSettings());

	_ui->previousDirectoriesComboBox->setVisible(false);
	
	_scanner.loadSettings();
}

ImageStackWidget::~ImageStackWidget() = default;

void ImageStackWidget::showEvent(QShowEvent* showEvent)
{
	_scanner.scan();

	QWidget::showEvent(showEvent);
}