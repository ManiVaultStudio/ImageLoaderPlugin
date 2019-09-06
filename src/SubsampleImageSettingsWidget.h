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

	void update();

private:
	std::unique_ptr<Ui::SubsampleImageSettingsWidget>	_ui;
	SubsampleImageSettings*								_subsampleImageSettings;
};	