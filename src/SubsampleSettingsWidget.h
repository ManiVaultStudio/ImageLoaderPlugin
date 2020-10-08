#pragma once

#include <memory>

#include <QWidget>

namespace Ui {
	class SubsampleSettingsWidget;
}

class ImageLoaderPlugin;

/**
 * Subsample settings widget class
 *
 * Widget class for image subsampling settings
 *
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
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	void initialize(ImageLoaderPlugin* imageLoaderPlugin);

private:
	std::unique_ptr<Ui::SubsampleSettingsWidget>	_ui;					/** Externally loaded UI */
	ImageLoaderPlugin*								_imageLoaderPlugin;		/** Pointer to image loader plugin (for interfacing with data models) */
};