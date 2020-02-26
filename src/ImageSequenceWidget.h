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
 * @author Thomas Kroes
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

	/** Returns the image scanner */
	ImageSequenceScanner& scanner() { return _scanner; }

	/** Returns the image loader */
	ImageLoader& loader() { return _loader; };

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;		/** Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;					/** Externally loaded UI */
	ImageSequenceScanner						_scanner;				/** Scanner */
	ImageLoader									_loader;				/** Loader */
};