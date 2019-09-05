#pragma once

#include "ResampleImageSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ResampleImageSettingsWidget;
}

class ResampleImageSettings;

class ResampleImageSettingsWidget : public QWidget
{
public:
	ResampleImageSettingsWidget(QWidget* parent);
	~ResampleImageSettingsWidget() override;

	void initialize(ResampleImageSettings* resampleImageSettings);

private:
	std::unique_ptr<Ui::ResampleImageSettingsWidget>	_ui;
	ResampleImageSettings*								_resampleImageSettings;
};	