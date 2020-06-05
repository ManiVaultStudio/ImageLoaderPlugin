#pragma once

#include "ImageLoader.h"
#include "ImageStackScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class StackSettingsWidget;
}

class ImageLoaderPlugin;

class QEvent;

/**
 * Image stack widget
 *
 * User interface widget for image stack loader/scanner settings
 *
 * @author Thomas Kroes
 */
class StackSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param parent Parent widget
	 */
	StackSettingsWidget(QWidget* parent);

	/** Destructor */
	~StackSettingsWidget();

public: // Initialization

	/**
	 * Initializes the widget with a loader plugin
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	void initialize(ImageLoaderPlugin* imageLoaderPlugin);

public: // Miscellaneous

	/** Returns the image scanner */
	ImageStackScanner& scanner() { return _scanner; }

	/** Returns the image loader */
	ImageLoader& loader() { return _loader; };

public: // Miscellaneous

	/**
	 * Invoked when the widget is show on the screen
	 * @param showEvent Show event
	 */
	void showEvent(QShowEvent* showEvent);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;		/** Pointer to image loader plugin (for interfacing with data models) */
	std::unique_ptr<Ui::StackSettingsWidget>	_ui;					/** UI from Qt creator */
	ImageStackScanner							_scanner;				/** Image sequence(s) scanner */
	ImageLoader									_loader;				/** Image sequence loader */
};