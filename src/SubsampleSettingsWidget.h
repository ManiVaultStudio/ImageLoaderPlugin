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
	std::unique_ptr<Ui::SubsampleSettingsWidget>	_ui;
	SubsampleSettings*								_subsampleSettings;
};