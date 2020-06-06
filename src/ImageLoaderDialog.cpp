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
	switch (_ui->closeAfterLoadedCheckBox->checkState())
	{
		case Qt::Checked:
			_settings.setValue("CloseAfterLoaded", true);
			break;

		case Qt::Unchecked:
			_settings.setValue("CloseAfterLoaded", false);
			break;

		default:
			break;
	}
}

void ImageLoaderDialog::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	_ui->commonSettingsWidget->initialize(_imageLoaderPlugin);

	const auto closeAfterLoaded = _settings.value("CloseAfterLoaded", true).toBool();

	_ui->closeAfterLoadedCheckBox->setCheckState(closeAfterLoaded ? Qt::Checked : Qt::Unchecked);

	auto& imageCollectionsModel = _imageLoaderPlugin->imageCollectionsModel();

	QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [this, &imageCollectionsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows	= imageCollectionsModel.selectionModel().selectedRows();
		const auto hasSelection	= !selectedRows.isEmpty();

		_ui->loadPushButton->setEnabled(false);

		if (hasSelection) {
			const auto imageCollectionType	= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollectionsModel::Column::Type)), Qt::DisplayRole).toString();
			const auto noSelectedImages		= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollectionsModel::Column::NoSelectedImages)), Qt::EditRole).toInt();

			_ui->loadPushButton->setEnabled(noSelectedImages > 0);
			_ui->loadPushButton->setText(QString("Load %1").arg(imageCollectionType));
		}
	});

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles /*= QVector<int>()*/) {
		auto& imageCollectionsModel = _imageLoaderPlugin->imageCollectionsModel();

		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();

		if (!selectedRows.isEmpty() && selectedRows.first().row() == topLeft.row()) {
			const auto imageCollectionType	= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollectionsModel::Column::Type)), Qt::DisplayRole).toString();
			const auto noSelectedImages		= imageCollectionsModel.data(selectedRows.first().siblingAtColumn(ult(ImageCollectionsModel::Column::NoSelectedImages)), Qt::EditRole).toInt();

			_ui->loadPushButton->setEnabled(noSelectedImages > 0);
			_ui->loadPushButton->setText(QString("Load %1").arg(imageCollectionType));
		}
	});
}