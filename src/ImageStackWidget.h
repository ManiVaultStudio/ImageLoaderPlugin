#pragma once

#include <memory>

#include <QWidget>

#include "ImageStack.h"

namespace Ui {
	class ImageStackWidget;
}

class ImageLoader;

class ImageStackWidget : public QWidget
{
public:
	ImageStackWidget(ImageLoader *imageLoader);
	~ImageStackWidget();

private slots:
	/*
	void onBecameDirty();
	void onBeginScan();
	void onEndScan();
	void onMessage(const QString &message);
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);
	void onScan();
	void onPickDirectory();
	void onImageTypeChanged(const QString &imageType);
	void onBeginLoad();
	void onEndLoad();
	*/

private:
	ImageLoader*							_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	ImageStack								_imageSequence;
};	