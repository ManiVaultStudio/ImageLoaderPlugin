#pragma once

#include <memory>

#include <QWidget>

#include "ImageStacks.h"

namespace Ui {
	class ImageStackWidget;
}

class ImageLoader;

class ImageStackWidget : public QWidget
{
public:
	ImageStackWidget(ImageLoader* imageLoader);
	~ImageStackWidget();

private slots:
	void onBecameDirty();
	void onBeginScan();
	void onEndScan();
	void onMessage(const QString &message);
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onPickDirectory();
	void onBeginLoad(ImageStack* imageStack);
	void onEndLoad(ImageStack* imageStack, std::vector<float>& pointsData);

private:
	ImageLoader*							_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	ImageStacks								_imageStacks;
};	