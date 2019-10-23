#pragma once

#include "ColorSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ColorSettingsWidget;
}

class ColorSettings;

class ColorSettingsWidget : public QWidget
{
public:
	ColorSettingsWidget(QWidget* parent);

	void initialize(ColorSettings* colorSettings);

private:
	void onConvertToGrayscaleChanged(const bool& convertToGrayscaleChanged);
	void onConvertToGrayscaleCheckBoxStateChanged(const int& state);

private:
	std::unique_ptr<Ui::ColorSettingsWidget>	_ui;
	ColorSettings*								_colorSettings;
};	