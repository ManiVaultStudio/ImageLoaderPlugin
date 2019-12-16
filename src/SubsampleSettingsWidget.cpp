#include "SubsampleSettingsWidget.h"

#include "ui_SubsampleSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

SubsampleSettingsWidget::SubsampleSettingsWidget(QWidget* parent) :
	_ui{ std::make_unique<Ui::SubsampleSettingsWidget>() },
	_subsampleSettings(nullptr)
{
	_ui->setupUi(this);
}

void SubsampleSettingsWidget::initialize(SubsampleSettings* subsampleSettings)
{
	_subsampleSettings = subsampleSettings;

	_ui->filterComboBox->addItems(_subsampleSettings->filterNames());

	connect(_subsampleSettings, &SubsampleSettings::enabledChanged, this, &SubsampleSettingsWidget::onEnabledChanged);
	connect(_ui->enableSubsamplingCheckbox, &QCheckBox::stateChanged, this, &SubsampleSettingsWidget::onEnabledCheckBoxStateChanged);

	connect(_subsampleSettings, &SubsampleSettings::ratioChanged, this, &SubsampleSettingsWidget::onRatioChanged);
	connect(_ui->ratioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SubsampleSettingsWidget::onRatioSpinBoxValueChanged);
	connect(_ui->ratioSlider, &QSlider::valueChanged, this, &SubsampleSettingsWidget::onRatioSliderValueChanged);
	connect(_ui->filterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SubsampleSettingsWidget::onFilterValueChanged);

	_subsampleSettings->emitAll();
}

void SubsampleSettingsWidget::onEnabledChanged(const bool& enabled)
{
	_ui->enableSubsamplingCheckbox->setChecked(enabled);

	_ui->ratioLabel->setEnabled(enabled);
	_ui->ratioSpinBox->setEnabled(enabled);
	_ui->ratioSlider->setEnabled(enabled);
	_ui->filterLabel->setEnabled(enabled);
	_ui->filterComboBox->setEnabled(enabled);
}

void SubsampleSettingsWidget::onEnabledCheckBoxStateChanged(const int& state)
{
	const auto enabled = static_cast<bool>(state);

	_subsampleSettings->setEnabled(enabled);
}

void SubsampleSettingsWidget::onRatioChanged(const double & ratio)
{
	if (ratio != _ui->ratioSpinBox->value()) {
		_ui->ratioSpinBox->blockSignals(true);
		_ui->ratioSpinBox->setValue(ratio);
		_ui->ratioSpinBox->blockSignals(false);
	}
	
	if (ratio != _ui->ratioSlider->value()) {
		_ui->ratioSlider->blockSignals(true);
		_ui->ratioSlider->setValue(ratio);
		_ui->ratioSlider->blockSignals(true);
	}
}

void SubsampleSettingsWidget::onRatioSpinBoxValueChanged(const double& ratio)
{
	_subsampleSettings->setRatio(ratio);
}

void SubsampleSettingsWidget::onRatioSliderValueChanged(const int& ratio)
{
	_subsampleSettings->setRatio(ratio);
}

void SubsampleSettingsWidget::onFilterValueChanged(const int& filter)
{
	_subsampleSettings->setFilter(static_cast<ImageResamplingFilter>(filter));
}

void SubsampleSettingsWidget::onFilterChanged(const ImageResamplingFilter& imageResamplingFilter)
{
	const int filterIndex = static_cast<int>(imageResamplingFilter);

	if (filterIndex == _ui->filterComboBox->currentIndex())
		return;

	_ui->filterComboBox->blockSignals(true);
	_ui->filterComboBox->setCurrentIndex(filterIndex);
	_ui->filterComboBox->blockSignals(false);
}