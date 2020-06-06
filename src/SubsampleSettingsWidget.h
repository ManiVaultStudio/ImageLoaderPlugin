#pragma once

#include <memory>

#include <QWidget>

namespace Ui {
	class SubsampleSettingsWidget;
}

class ImageLoaderPlugin;

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
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	void initialize(ImageLoaderPlugin* imageLoaderPlugin);

	/**
	 * Updates the user interface with data from the given layer model indices
	 * @param topLeft Top left model index
	 * @param bottomRight Bottom right model index
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());

private:
	ImageLoaderPlugin*								_imageLoaderPlugin;		/** Pointer to image loader plugin (for interfacing with data models) */
	std::unique_ptr<Ui::SubsampleSettingsWidget>	_ui;					/** Externally loaded UI */
};