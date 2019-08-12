#pragma once

#include <memory>

#include <QWidget>

#include "ImageSequence.h"

namespace Ui {
	class ImageSequenceWidget;
}

class ImageLoaderPlugin;

class ImageSequenceWidget : public QWidget
{
public:
	ImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);
	~ImageSequenceWidget() override;

private:
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

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;
	ImageSequence								_imageSequence;
};