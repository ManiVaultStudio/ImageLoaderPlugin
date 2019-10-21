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
	void onEndScan(const ImageCollections& scannedImageCollections);
	void onDirectoryChanged(const QString &directory);
	void onLoadPushButtonClicked();
	void onDatasetNameChanged(const QString& text);
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);
	void onPickDirectory();
	void onImageTypeChanged(const QString &imageType);
	void onBeginLoad();
	void onEndLoad(Images& imagePointDataSet);
	
signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;
	ImageSequenceScanner						_scanner;
	ImageCollectionsLoader						_loader;
};