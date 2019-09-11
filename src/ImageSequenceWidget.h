#pragma once

#include "ImageCollectionsLoader.h"
#include "ImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ImageSequenceWidget;
}

class ImageLoaderPlugin;

class ImageSequenceWidget : public QWidget
{
	Q_OBJECT

public:
	ImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);
	~ImageSequenceWidget() override;

private:
	void onBecameDirty();
	void onBeginScan();
	void onEndScan(ImageCollections& imageCollections);
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);
	void onPickDirectory();
	void onImageTypeChanged(const QString &imageType);
	void onBeginLoad();
	void onEndLoad(ImageCollections& imageCollections);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;
	ImageSequenceScanner						_scanner;
	ImageCollections							_scanned;
	ImageCollectionsLoader						_loader;
};