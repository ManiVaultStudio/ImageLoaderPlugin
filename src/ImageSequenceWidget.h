#pragma once

#include <memory>

#include <QWidget>

#include "ImageSequence.h"

namespace Ui {
	class ImageSequenceWidget;
}

class ImageLoader;

class ImageSequenceWidget : public QWidget
{
public:
	ImageSequenceWidget(ImageLoader *imageLoader);
	~ImageSequenceWidget();

private slots:
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
	ImageLoader*								_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;
	ImageSequence								_imageSequence;
};