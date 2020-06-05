#pragma once

#include "ImageLoader.h"
#include "ImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class SequenceSettingsWidget;
}

class ImageLoaderPlugin;

/**
 * Image sequence widget
 *
 * User interface widget for image sequence loader/scanner settings
 *
 * @author Thomas Kroes
 */
class SequenceSettingsWidget : public QWidget
{
	Q_OBJECT

public: // Construction/destruction

	/**
	 * Constructor 
	 * @param parent Parent widget
	 */
	SequenceSettingsWidget(QWidget* parent);

	/** Destructor */
	~SequenceSettingsWidget();

public: // Initialization

	/**
	 * Initializes the widget
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	void initialize(ImageLoaderPlugin* imageLoaderPlugin);

public: // Miscellaneous

	/** Returns the image scanner */
	ImageSequenceScanner& scanner() { return _scanner; }

	/** Returns the image loader */
	ImageLoader& loader() { return _loader; };

private:
	ImageLoaderPlugin*								_imageLoaderPlugin;			/** Pointer to image loader plugin (for interfacing with data models) */
	std::unique_ptr<Ui::SequenceSettingsWidget>		_ui;						/** Externally loaded UI */
	ImageSequenceScanner							_scanner;					/** Scanner */
	ImageLoader										_loader;					/** Loader */
};