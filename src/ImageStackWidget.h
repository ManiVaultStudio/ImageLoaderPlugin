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

/*!
	\class ImageStackWidget
	\inherits QWidget
	\brief Provides a UI widget for interfacing with \a scanner and \a loader parameters
	\relates ImageStackScanner
	\relates ImageLoader
	\ingroup ImageLoaderPlugin
*/
class ImageStackWidget : public QWidget
{
	Q_OBJECT

public:
	ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin);
    ~ImageStackWidget();

private:
	void showEvent(QShowEvent* showEvent);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;		/*!< Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::ImageStackWidget>	_ui;					/*!< Externally loaded UI */
	ImageStackScanner						_scanner;				/*!< Scanner */
	ImageLoader								_loader;				/*!< Loader */
};