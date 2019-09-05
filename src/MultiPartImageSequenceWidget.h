#pragma once

#include "MultipartImageSequence.h"

#include <memory>

#include <QWidget>

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
	MultipartImageSequence								_multipartImageSequence;
};