#pragma once

#include "ImageLoader.h"
#include "MultiPartImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class MultiPartImageSequenceWidget;
}

class ImageLoaderPlugin;

/*!
	\class MultiPartImageSequenceWidget
	\inherits QWidget
	\brief Provides a UI widget for interfacing with \a scanner and \a loader parameters
	\relates MultiPartImageSequenceScanner
	\relates ImageLoader
	\ingroup ImageLoaderPlugin
*/
class MultiPartImageSequenceWidget : public QWidget
{
	Q_OBJECT

public:
	MultiPartImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);
	~MultiPartImageSequenceWidget();

private:
	void showEvent(QShowEvent* showEvent);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*									_imageLoaderPlugin;		/*!< Pointer to the ImageLoaderPlugin */
	std::unique_ptr<Ui::MultiPartImageSequenceWidget>	_ui;					/*!< Externally loaded UI */
	MultiPartImageSequenceScanner						_scanner;				/*!< Scanner */
	ImageLoader											_loader;				/*!< Loader */
};