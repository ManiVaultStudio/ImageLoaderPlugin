#pragma once

#include "ImageLoader.h"
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

private:
	void onScannerSettingsChanged();
	void onLoaderSettingsChanged();
	void onBeginScan();
	void onEndScan(std::shared_ptr<ImageCollections> scanned);
	void onDirectoryChanged(const QString &directory);
	void onLoadPushButtonClicked();
	void onDatasetNameChanged(const QString& text);
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);
	void onPickDirectory();
	void onImageTypeChanged(const QString &imageType);
	void onBeginLoad();
	void onEndLoad(std::shared_ptr<Payload> payload);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;
	ImageSequenceScanner						_scanner;
	ImageLoader						_loader;
};