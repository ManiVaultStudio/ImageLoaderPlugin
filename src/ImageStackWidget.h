#pragma once

#include "ImageStack.h"
#include "ImageStackScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ImageStackWidget;
}

class ImageLoaderPlugin;

class ImageStackWidget : public QWidget
{
public:
	ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin);

private:
	void onBecameDirty();
	void onBeginScan();
	void onEndScan();
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onPickDirectory();
	void onBeginLoad(ImageStack* imageStack);
	void onEndLoad(ImageStack* imageStack, std::vector<float>& pointsData);

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	ImageStack								_imageStack;
	ImageStackScanner						_imageStackScanner;
};	