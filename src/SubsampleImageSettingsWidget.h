#pragma once

#include "SubsampleImageSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class SubsampleImageSettingsWidget;
}

class ResampleImageSettings;

class SubsampleImageSettingsWidget : public QWidget
{
public:
	SubsampleImageSettingsWidget(QWidget* parent);
	~SubsampleImageSettingsWidget() override;

	void initialize(SubsampleImageSettings* subsampleImageSettings);

private:
	void onEnabledChanged(const bool& enabled);
	void onEnabledCheckBoxStateChanged(const int& state);
	void onRatioChanged(const double& ratio);
	void onRatioSpinBoxValueChanged(const double& ratio);
	void onRatioSliderValueChanged(const int& ratio);
	void onFilterChanged(const ImageResamplingFilter& imageResamplingFilter);

private:
	std::unique_ptr<Ui::SubsampleImageSettingsWidget>	_ui;
	SubsampleImageSettings*								_subsampleImageSettings;
};	