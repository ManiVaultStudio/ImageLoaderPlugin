#include "SubsampleImageSettingsWidget.h"

#include "ui_SubsampleImageSettingsWidget.h"

#include <QDebug>
#include <QFileDialog>

#include "ImageLoaderPlugin.h"

SubsampleImageSettingsWidget::SubsampleImageSettingsWidget(QWidget* parent) :
	_ui{ std::make_unique<Ui::SubsampleImageSettingsWidget>() },
	_subsampleImageSettings(nullptr)
{
	_ui->setupUi(this);
}

void SubsampleImageSettingsWidget::initialize(SubsampleImageSettings* subsampleImageSettings)
{
	_subsampleImageSettings = subsampleImageSettings;

	_ui->filterComboBox->addItems(_subsampleImageSettings->filterNames());

	connect(_subsampleImageSettings, &SubsampleImageSettings::enabledChanged, this, &SubsampleImageSettingsWidget::onEnabledChanged);
	connect(_ui->enableSubsamplingCheckbox, &QCheckBox::stateChanged, this, &SubsampleImageSettingsWidget::onEnabledCheckBoxStateChanged);

	connect(_subsampleImageSettings, &SubsampleImageSettings::ratioChanged, this, &SubsampleImageSettingsWidget::onRatioChanged);
	connect(_ui->ratioSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SubsampleImageSettingsWidget::onRatioSpinBoxValueChanged);
	connect(_ui->ratioSlider, &QSlider::valueChanged, this, &SubsampleImageSettingsWidget::onRatioSliderValueChanged);

	_subsampleImageSettings->emitAll();
}

void SubsampleImageSettingsWidget::onEnabledChanged(const bool& enabled)
{
	_ui->enableSubsamplingCheckbox->setChecked(enabled);

	_ui->ratioLabel->setEnabled(enabled);
	_ui->ratioSpinBox->setEnabled(enabled);
	_ui->ratioSlider->setEnabled(enabled);
	_ui->filterLabel->setEnabled(enabled);
	_ui->filterComboBox->setEnabled(enabled);
}

void SubsampleImageSettingsWidget::onEnabledCheckBoxStateChanged(const int& state)
{
	const auto enabled = static_cast<bool>(state);

	_subsampleImageSettings->setEnabled(enabled);
}

void SubsampleImageSettingsWidget::onRatioChanged(const double & ratio)
{
	if (ratio != _ui->ratioSpinBox->value()) {
		_ui->ratioSpinBox->setValue(ratio);
	}
	
	if (ratio != _ui->ratioSlider->value()) {
		_ui->ratioSlider->setValue(ratio);
	}
}

void SubsampleImageSettingsWidget::onRatioSpinBoxValueChanged(const double& ratio)
{
	_subsampleImageSettings->setRatio(ratio);
}

void SubsampleImageSettingsWidget::onRatioSliderValueChanged(const int& ratio)
{
	_subsampleImageSettings->setRatio(ratio);
}

void SubsampleImageSettingsWidget::onFilterChanged(const ImageResamplingFilter& imageResamplingFilter)
{
	const int filterIndex = static_cast<int>(imageResamplingFilter);

	if (filterIndex == _ui->filterComboBox->currentIndex())
		return;

	_ui->filterComboBox->setCurrentIndex(filterIndex);
}