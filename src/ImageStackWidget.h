#pragma once

#include "ImageLoader.h"
#include "ImageStackScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ImageStackWidget;
}

class ImageLoaderPlugin;

class QEvent;

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
	void onEndScan(std::shared_ptr<ImageCollections> scanned);
	void onBeginLoad();
	void onEndLoad(std::shared_ptr<Payload> payload);
	void onSubsampleImageSettingsChanged();

	void showEvent(QShowEvent* showEvent);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	ImageStackScanner						_scanner;
	ImageLoader					_loader;
};