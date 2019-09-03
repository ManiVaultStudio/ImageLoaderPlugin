#pragma once

#include <memory>

#include <QWidget>

#include "ImageSequence.h"

namespace Ui {
	class MultiPartImageSequenceWidget;
}

class ImageLoaderPlugin;

class MultiPartImageSequenceWidget : public QWidget
{
public:
	MultiPartImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);
	~MultiPartImageSequenceWidget() override;

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
	ImageLoaderPlugin*									_imageLoaderPlugin;
	std::unique_ptr<Ui::MultiPartImageSequenceWidget>	_ui;
	//ImageSequence								_imageSequence;
};