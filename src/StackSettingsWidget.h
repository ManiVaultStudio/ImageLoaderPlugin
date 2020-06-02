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
 * User interface widget for image stack loader/scanner settings
 * @author Thomas Kroes
 */
class StackSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param parent Parent object
	 */
	StackSettingsWidget(QObject* parent);

	/** Destructor */
	~StackSettingsWidget();

	/** Returns the image scanner */
	ImageStackScanner& scanner() { return _scanner; }

	/** Returns the image loader */
	ImageLoader& loader() { return _loader; };

private:
	/**
	 * Invoked when the widget is show on the screen
	 * @param showEvent Show event
	 */
	void showEvent(QShowEvent* showEvent);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;		/** Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::StackSettingsWidget>	_ui;					/** Externally loaded UI */
	ImageStackScanner							_scanner;				/** Scanner */
	ImageLoader									_loader;				/** Loader */
};