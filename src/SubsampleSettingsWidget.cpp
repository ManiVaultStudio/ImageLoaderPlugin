#include "SubsampleSettingsWidget.h"
#include "ImageLoaderPlugin.h"

#include "ui_SubsampleSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

SubsampleSettingsWidget::SubsampleSettingsWidget(QWidget* parent) :
	_ui{ std::make_unique<Ui::SubsampleSettingsWidget>() },
	_imageLoaderPlugin(nullptr)
{
	_ui->setupUi(this);
}

void SubsampleSettingsWidget::initialize(ImageLoaderPlugin* imageLoaderPlugin)
{
	_imageLoaderPlugin = imageLoaderPlugin;

	auto& imageCollectionsModel				= _imageLoaderPlugin->getImageCollectionsModel();
	auto& imageCollectionsSelectionModel	= imageCollectionsModel.selectionModel();
	auto& filterModel						= _imageLoaderPlugin->getImageCollectionsFilterModel();

	auto selectedRow = [&]() {
		const auto selectedRows = imageCollectionsSelectionModel.selectedRows();
		
		if (selectedRows.isEmpty())
			return QModelIndex();

		return filterModel.mapToSource(selectedRows.first());
	};

	auto updateData = [&](const QModelIndex& topLeft, const QModelIndex& bottomRight)
	{
		const auto selectedRows = imageCollectionsModel.selectionModel().selectedRows();
		const auto validSelection = selectedRows.size() == 1 && filterModel.mapToSource(selectedRows.first()).row() == topLeft.row();
		const auto subsamplingEnabled = imageCollectionsModel.data(topLeft.siblingAtColumn(ult(ImageCollection::Column::SubsamplingEnabled)), Qt::EditRole).toBool();
		const auto mightEdit = validSelection && subsamplingEnabled;

		auto columnStart = topLeft.column();
		auto columnEnd = bottomRight.column();

		if (topLeft == QModelIndex() && bottomRight == QModelIndex()) {
			columnStart = ult(ImageCollection::Column::SubsamplingEnabled);
			columnEnd = ult(ImageCollection::Column::SubsamplingFilter);
		}

		for (int column = columnStart; column <= columnEnd; column++) {
			if (column == ult(ImageCollection::Column::SubsamplingEnabled)) {

				_ui->enabledCheckbox->blockSignals(true);
				_ui->enabledCheckbox->setChecked(subsamplingEnabled);
				_ui->enabledCheckbox->blockSignals(false);

				_ui->enabledCheckbox->setEnabled(validSelection);
				_ui->ratioLabel->setEnabled(mightEdit);
				_ui->ratioSpinBox->setEnabled(mightEdit);
				_ui->ratioSlider->setEnabled(mightEdit);
				_ui->ratio25PushButton->setEnabled(mightEdit);
				_ui->ratio50PushButton->setEnabled(mightEdit);
				_ui->ratio75PushButton->setEnabled(mightEdit);
				_ui->ratio100PushButton->setEnabled(mightEdit);
				_ui->filterLabel->setEnabled(mightEdit);
				_ui->filterComboBox->setEnabled(mightEdit);
			}

			if (column == ult(ImageCollection::Column::SubsamplingRatio)) {
				const auto subsamplingRatio = imageCollectionsModel.data(topLeft.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), Qt::EditRole).toFloat();

				_ui->ratioSpinBox->blockSignals(true);
				_ui->ratioSpinBox->setValue(validSelection ? subsamplingRatio * 100.0f : 50.0f);
				_ui->ratioSpinBox->blockSignals(false);

				_ui->ratioSlider->blockSignals(true);
				_ui->ratioSlider->setValue(validSelection ? subsamplingRatio * 100.0f : 50.0f);
				_ui->ratioSlider->blockSignals(false);
			}

			if (column == ult(ImageCollection::Column::SubsamplingFilter)) {
				const auto subsamplingFilter = imageCollectionsModel.data(topLeft.siblingAtColumn(ult(ImageCollection::Column::SubsamplingFilter)), Qt::EditRole).toInt();

				_ui->filterComboBox->blockSignals(true);
				_ui->filterComboBox->setCurrentIndex(validSelection ? subsamplingFilter : 0);
				_ui->filterComboBox->blockSignals(false);
			}
		}
	};

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::dataChanged, this, updateData);

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::rowsInserted, [this, updateData]() {
		updateData(QModelIndex(), QModelIndex());
	});

	QObject::connect(&imageCollectionsModel, &ImageCollectionsModel::modelReset, [this, updateData]() {
		updateData(QModelIndex(), QModelIndex());
	});

	QObject::connect(&imageCollectionsModel.selectionModel(), &QItemSelectionModel::selectionChanged, [&, selectedRow, updateData](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto index = selectedRow();

		if (index == QModelIndex())
			updateData(QModelIndex(), QModelIndex());
		else {
			updateData(index.siblingAtColumn(ult(ImageCollection::Column::Start)), index.siblingAtColumn(ult(ImageCollection::Column::End)));
		}
	});

	QObject::connect(_ui->enabledCheckbox, &QCheckBox::stateChanged, [&, selectedRow](int state) {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingEnabled)), _ui->enabledCheckbox->isChecked());
	});

	connect(_ui->ratioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&, selectedRow](double ratio) {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 0.01f * ratio);
	});

	connect(_ui->ratioSlider, &QSlider::valueChanged, [&, selectedRow](int ratio) {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 0.01f * ratio);
	});

	connect(_ui->ratio25PushButton, &QPushButton::clicked, [&, selectedRow]() {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 0.25f);
	});

	connect(_ui->ratio50PushButton, &QPushButton::clicked, [&, selectedRow]() {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 0.5f);
	});

	connect(_ui->ratio75PushButton, &QPushButton::clicked, [&, selectedRow]() {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 0.75f);
	});

	connect(_ui->ratio100PushButton, &QPushButton::clicked, [&, selectedRow]() {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 1.0f);
	});

	QObject::connect(_ui->filterComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [&, selectedRow](int currentIndex) {
		const auto index = selectedRow();

		if (index != QModelIndex())
			imageCollectionsModel.setData(index.siblingAtColumn(ult(ImageCollection::Column::SubsamplingFilter)), currentIndex);
	});

	updateData(QModelIndex(), QModelIndex());
}