#pragma once

#include "SubsampleSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class SubsampleSettingsWidget;
}

/**
 * Subsample settings widget
 * User interface widget for image subsampling settings
 */
class SubsampleSettingsWidget : public QWidget
{
public:
	/**
	 * Constructor
	 * @param parent Parent widget
	 */
	SubsampleSettingsWidget(QWidget* parent);

	/** Initializes the subsample settings */
	void initialize(SubsampleSettings* subsampleSettings);

private:
	std::unique_ptr<Ui::SubsampleSettingsWidget>	_ui;					/** Externally loaded UI */
	SubsampleSettings*								_subsampleSettings;		/** Subsample settings */
};