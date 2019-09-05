#pragma once

#include <memory>

#include <QWidget>

#include "ImageStacks.h"

namespace Ui {
	class ImageStackWidget;
}

class ImageLoaderPlugin;

class ImageStackWidget : public QWidget
{
public:
	ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin);
	~ImageStackWidget() override;

private:
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
	ImageLoaderPlugin*						_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	//ImageStacks								_imageStacks;
};	