#pragma once

#include "ImageLoader.h"
#include "MultiPartImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class MultiPartImageSequenceWidget;
}

class ImageLoaderPlugin;

/**
 * Multi-part image sequence widget
 * User interface widget for multi-part image sequences loader/scanner settings
 * @author Thomas Kroes
 */
class MultiPartImageSequenceWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	MultiPartImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);

	/** Destructor */
	~MultiPartImageSequenceWidget();

	/** Returns the image scanner */
	MultiPartImageSequenceScanner& scanner() { return _scanner; }

	/** Returns the image loader */
	ImageLoader& loader() { return _loader; };

private:
	/**
	 * Invoked when the widget is show on the screen
	 * @param showEvent Show event
	 */
	void showEvent(QShowEvent* showEvent);

private:
	ImageLoaderPlugin*									_imageLoaderPlugin;		/** Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::MultiPartImageSequenceWidget>	_ui;					/** Externally loaded UI */
	MultiPartImageSequenceScanner						_scanner;				/** Scanner */
	ImageLoader											_loader;				/** Loader */
};