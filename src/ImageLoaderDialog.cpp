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
}

ImageLoaderDialog::~ImageLoaderDialog() = default;

void ImageLoaderDialog::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_ui->commonSettingsWidget->initialize(_imageLoaderPlugin);
	_ui->subsampleSettingswidget->initialize(_imageLoaderPlugin);

	_ui->closeAfterLoadedCheckBox->setChecked(_settings.value("CloseAfterLoaded", true).toBool());

	auto& imageCollectionsModel				= _imageLoaderPlugin->getImageCollectionsModel();
	auto& imageCollectionsSelectionModel	= imageCollectionsModel.selectionModel();
	auto& filterModel						= _imageLoaderPlugin->getImageCollectionsFilterModel();

	const auto selectedRow = [&]() {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();

		if (selectedRows.isEmpty())
			return QModelIndex();

		return filterModel.mapToSource(selectedRows.first());
	};

	const auto updateLoadButton = [&, selectedRow]() {
		const auto index = selectedRow();

		_ui->loadPushButton->setEnabled(false);

		if (index != QModelIndex()) {
			const auto imageCollectionType	= imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::DisplayRole).toString();
			const auto noSelectedImages		= imageCollectionsModel.data(index.siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages)), Qt::EditRole).toInt();

			_ui->loadPushButton->setEnabled(noSelectedImages > 0);
			_ui->loadPushButton->setText(QString("Load %1").arg(imageCollectionType));
		}
	};

	QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [&, selectedRow, updateLoadButton](const QItemSelection& selected, const QItemSelection& deselected) {
		updateLoadButton();
	});

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, [&, selectedRow, updateLoadButton](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles) {
		updateLoadButton();
	});

	QObject::connect(_ui->closeAfterLoadedCheckBox, &QPushButton::clicked, [&]() {
		_settings.setValue("CloseAfterLoaded", _ui->closeAfterLoadedCheckBox->isChecked());
	});

	QObject::connect(_ui->loadPushButton, &QPushButton::clicked, [&, selectedRow]() {
		const auto index = selectedRow();

		if (index != QModelIndex()) {
			const auto loaded = imageCollectionsModel.loadImageCollection(_imageLoaderPlugin, index);

			if (loaded && _settings.value("CloseAfterLoaded", true).toBool())
				this->close();
		}
	});
}