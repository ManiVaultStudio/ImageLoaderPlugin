#pragma once

#include "SubsampleSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class SubsampleSettingsWidget;
}

class SubsampleSettingsWidget : public QWidget
{
public:
	SubsampleSettingsWidget(QWidget* parent);

	void initialize(SubsampleSettings* subsampleSettings);

private:
	void onEnabledChanged(const bool& enabled);
	void onEnabledCheckBoxStateChanged(const int& state);
	void onRatioChanged(const double& ratio);
	void onRatioSpinBoxValueChanged(const double& ratio);
	void onRatioSliderValueChanged(const int& ratio);
	void onFilterChanged(const ImageResamplingFilter& imageResamplingFilter);

private:
	std::unique_ptr<Ui::SubsampleSettingsWidget>	_ui;
	SubsampleSettings*								_subsampleSettings;
};