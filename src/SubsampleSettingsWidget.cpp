#include "SubsampleSettingsWidget.h"

#include "ui_SubsampleSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

SubsampleSettingsWidget::SubsampleSettingsWidget(QWidget* parent) :
	_ui{ std::make_unique<Ui::SubsampleSettingsWidget>() }
{
	_ui->setupUi(this);
}

void SubsampleSettingsWidget::initialize()
{
	/*
	_subsampleSettings = subsampleSettings;

	_ui->filterComboBox->addItems(_subsampleSettings->filterNames());

	connect(_subsampleSettings, &SubsampleSettings::enabledChanged, [&](bool enabled) {
		_ui->enableSubsamplingCheckbox->blockSignals(true);
		_ui->enableSubsamplingCheckbox->setChecked(enabled);
		_ui->enableSubsamplingCheckbox->blockSignals(false);

		_ui->ratioLabel->setEnabled(enabled);
		_ui->ratioSpinBox->setEnabled(enabled);
		_ui->ratioSlider->setEnabled(enabled);
		_ui->filterLabel->setEnabled(enabled);
		_ui->filterComboBox->setEnabled(enabled);
	});

	connect(_ui->enableSubsamplingCheckbox, &QCheckBox::stateChanged, [&](int state) {
		_subsampleSettings->setEnabled(static_cast<bool>(state));
	});

	connect(_subsampleSettings, &SubsampleSettings::ratioChanged, [&](const double& ratio) {
		if (ratio != _ui->ratioSpinBox->value()) {
			_ui->ratioSpinBox->blockSignals(true);
			_ui->ratioSpinBox->setValue(100.0 * ratio);
			_ui->ratioSpinBox->blockSignals(false);
		}

		if (ratio != _ui->ratioSlider->value()) {
			_ui->ratioSlider->blockSignals(true);
			_ui->ratioSlider->setValue(100.0 * ratio);
			_ui->ratioSlider->blockSignals(false);
		}
	});

	connect(_ui->ratioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&](double ratio) {
		_subsampleSettings->setRatio(0.01 * ratio);
	});

	connect(_ui->ratioSlider, &QSlider::valueChanged, [&](int ratio) {
		_subsampleSettings->setRatio(static_cast<double>(0.01 * ratio));
	});

	connect(_ui->filterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int filter) {
		_subsampleSettings->setFilter(static_cast<ImageResamplingFilter>(filter));
	});

	connect(_subsampleSettings, &SubsampleSettings::filterChanged, [&](const ImageResamplingFilter& imageResamplingFilter) {
		const int filterIndex = static_cast<int>(imageResamplingFilter);

		if (filterIndex == _ui->filterComboBox->currentIndex())
			return;

		_ui->filterComboBox->blockSignals(true);
		_ui->filterComboBox->setCurrentIndex(filterIndex);
		_ui->filterComboBox->blockSignals(false);
	});

	_subsampleSettings->loadSettings();
	*/
}