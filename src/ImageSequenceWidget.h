#pragma once

#include "ImageLoader.h"
#include "ImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ImageSequenceWidget;
}

class ImageLoaderPlugin;

/*!
	\class ImageSequenceWidget
	\inherits QWidget
	\brief Provides a UI widget for interfacing with \a scanner and \a loader parameters
	\relates ImageSequenceScanner
	\relates ImageLoader
	\ingroup ImageLoaderPlugin
*/
class ImageSequenceWidget : public QWidget
{
	Q_OBJECT

public:
	ImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);
    ~ImageSequenceWidget();

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;		/*!< Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;					/*!< Externally loaded UI */
	ImageSequenceScanner						_scanner;				/*!< Scanner */
	ImageLoader									_loader;				/*!< Loader */
};