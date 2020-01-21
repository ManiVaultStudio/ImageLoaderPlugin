#pragma once

#include "ImageLoader.h"
#include "ImageStackScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ImageStackWidget;
}

class ImageLoaderPlugin;

class QEvent;

/**
 * Image stack widget
 * User interface widget for image stack loader/scanner settings
 * @author Thomas Kroes
 */
class ImageStackWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin);

	/** Destructor */
	~ImageStackWidget();

private:
	/**
	 * Invoked when the widget is show on the screen
	 * @param showEvent Show event
	 */
	void showEvent(QShowEvent* showEvent);

signals:
	/**
	 * Broadcasts a message
	 * @param message Message to broadcast
	 */
	void message(const QString& message);

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;		/** Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::ImageStackWidget>	_ui;					/** Externally loaded UI */
	ImageStackScanner						_scanner;				/** Scanner */
	ImageLoader								_loader;				/** Loader */
};