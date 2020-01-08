#pragma once

#include "SubsampleSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class SubsampleSettingsWidget;
}

/*!
	\class SubsampleSettingsWidget
	\inherits QWidget
	\brief UI for interfacing with image subsample settings
	\ingroup ImageLoaderPlugin
*/
class SubsampleSettingsWidget : public QWidget
{
public:
	SubsampleSettingsWidget(QWidget* parent);

	void initialize(SubsampleSettings* subsampleSettings);

private:
	std::unique_ptr<Ui::SubsampleSettingsWidget>	_ui;					/*!< Externally loaded UI */
	SubsampleSettings*								_subsampleSettings;		/*!< Subsample settings */
};