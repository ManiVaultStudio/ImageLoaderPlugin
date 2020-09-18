#pragma once

#include "ImageCollectionScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class CommonSettingsWidget;
}

class ImageLoaderPlugin;

/**
 * Common settings widget
 *
 * Widget class for common settings
 *
 * @author Thomas Kroes
 */
class CommonSettingsWidget : public QWidget
{
	Q_OBJECT

public: // Construction/destruction

	/**
	 * Constructor 
	 * @param parent Parent widget
	 */
	CommonSettingsWidget(QWidget* parent);

	/** Destructor */
	~CommonSettingsWidget();

public: // Initialization

	/**
	 * Initializes the widget
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	void initialize(ImageLoaderPlugin* imageLoaderPlugin);

public: // Miscellaneous

	/** Returns the image collection scanner */
	ImageCollectionScanner& getScanner() { return _scanner; }

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;		/** Pointer to image loader plugin (for interfacing with data models) */
	std::unique_ptr<Ui::CommonSettingsWidget>	_ui;					/** Externally loaded UI */
	QSettings									_settings;				/** Settings */
	ImageCollectionScanner						_scanner;				/** Image collection scanner */
};