#pragma once

#include <memory>

#include <QWidget>

namespace Ui {
	class SubsampleSettingsWidget;
}

/**
 * Subsample settings widget
 * User interface widget for image subsampling settings
 * @author Thomas Kroes
 */
class SubsampleSettingsWidget : public QWidget
{
public:
	/**
	 * Constructor
	 * @param parent Parent widget
	 */
	SubsampleSettingsWidget(QWidget* parent);

	/**
	 * Initializes the widget
	 * @param subsampleSettings Pointer to subsample settings
	 */
	void initialize();

private:
	std::unique_ptr<Ui::SubsampleSettingsWidget>	_ui;					/** Externally loaded UI */
};