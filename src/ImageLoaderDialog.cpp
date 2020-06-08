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
	setMinimumWidth(480);
	setMinimumHeight(600);

	_ui->setupUi(this);
}

ImageLoaderDialog::~ImageLoaderDialog()
{
	_settings.setValue("CloseAfterLoaded", _ui->closeAfterLoadedCheckBox->isChecked());
}

void ImageLoaderDialog::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_ui->commonSettingsWidget->initialize(_imageLoaderPlugin);
	_ui->subsampleSettingswidget->initialize(_imageLoaderPlugin);

	_ui->closeAfterLoadedCheckBox->setChecked(_settings.value("CloseAfterLoaded", true).toBool());

	auto& imageCollectionsModel = _imageLoaderPlugin->imageCollectionsModel();

	QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [this, &imageCollectionsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows	= imageCollectionsModel.selectionModel().selectedRows();
		const auto hasSelection	= !selectedRows.isEmpty();

		_ui->loadPushButton->setEnabled(false);

		if (hasSelection) {
			const auto imageCollectionType	= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::DisplayRole).toString();
			const auto noSelectedImages		= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages)), Qt::EditRole).toInt();

			_ui->loadPushButton->setEnabled(noSelectedImages > 0);
			_ui->loadPushButton->setText(QString("Load %1").arg(imageCollectionType));
		}
	});

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles) {
		auto& imageCollectionsModel = _imageLoaderPlugin->imageCollectionsModel();

		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty() && selectedRows.first().row() == topLeft.row()) {
			const auto imageCollectionType	= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollection::Column::Type)), Qt::DisplayRole).toString();
			const auto noSelectedImages		= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages)), Qt::EditRole).toInt();

			_ui->loadPushButton->setEnabled(noSelectedImages > 0);
			_ui->loadPushButton->setText(QString("Load %1").arg(imageCollectionType));
		}
	});
}