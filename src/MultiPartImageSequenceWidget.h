#pragma once

#include "ImageLoader.h"
#include "MultiPartImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class MultiPartImageSequenceWidget;
}

class ImageLoaderPlugin;

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
	ImageLoaderPlugin*									_imageLoaderPlugin;
	std::unique_ptr<Ui::MultiPartImageSequenceWidget>	_ui;
	MultiPartImageSequenceScanner						_scanner;
	ImageLoader											_loader;
};