#pragma once

#include "ImageLoader.h"
#include "ImageStackScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ImageStackWidget;
}

class ImageLoaderPlugin;

class ImageStackWidget : public QWidget
{
	Q_OBJECT

public:
	ImageStackWidget(ImageLoaderPlugin* imageLoaderPlugin);

private:
	void onScannerSettingsChanged();
	void onLoaderSettingsChanged();
	void onPickDirectory();
	void onDirectoryChanged(const QString &directory);
	void onLoadPushButtonClicked();
	void onDatasetNameChanged(const QString& dataSetName);
	void onBeginScan();
	void onEndScan(const ImageCollections& scannedImageCollections);
	void onBeginLoad();
	void onEndLoad(Payload& payload);
	void onSubsampleImageSettingsChanged();

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	ImageStackScanner						_scanner;
	ImageLoader					_loader;
};