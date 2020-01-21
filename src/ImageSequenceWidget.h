#pragma once

#include "ImageLoader.h"
#include "ImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ImageSequenceWidget;
}

class ImageLoaderPlugin;

/**
 * Image sequence widget
 * User interface widget for image sequence loader/scanner settings
 */
class ImageSequenceWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	ImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);

	/** Destructor */
	~ImageSequenceWidget();

signals:
	/**
	 * Broadcasts a message
	 * @param message Message to broadcast
	 */
	void message(const QString& message);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;		/** Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;					/** Externally loaded UI */
	ImageSequenceScanner						_scanner;				/** Scanner */
	ImageLoader									_loader;				/** Loader */
};