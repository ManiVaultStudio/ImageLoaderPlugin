#pragma once

#include "ColorSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ColorSettingsWidget;
}

class ColorSettings;

/*!
	\class ColorSettingsWidget
	\inherits QWidget
	\brief UI for interfacing with color settings
	\ingroup ImageLoaderPlugin
*/
class ColorSettingsWidget : public QWidget
{
public:
	ColorSettingsWidget(QWidget* parent);

	void initialize(ColorSettings* colorSettings);

private:
	std::unique_ptr<Ui::ColorSettingsWidget>	_ui;				/*!< Externally loaded UI */
	ColorSettings*								_colorSettings;		/*!< Color settings */
};