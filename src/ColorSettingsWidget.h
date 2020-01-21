#pragma once

#include "ColorSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ColorSettingsWidget;
}

class ColorSettings;

/**
 * Color settings widget
 * User interface widget for color subsampling settings
 * @author Thomas Kroes
 */
class ColorSettingsWidget : public QWidget
{
public:
	/**
	 * Constructor
	 * @param parent Pointer to parent widget
	 */
	ColorSettingsWidget(QWidget* parent);

	/**
	 * Initializes the widget
	 * @param colorSettings Pointer to color settings
	 */
	void initialize(ColorSettings* colorSettings);

private:
	std::unique_ptr<Ui::ColorSettingsWidget>	_ui;				/*!< Externally loaded UI */
	ColorSettings*								_colorSettings;		/*!< Color settings */
};