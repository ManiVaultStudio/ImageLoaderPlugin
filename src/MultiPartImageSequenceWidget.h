#pragma once

#include "ImageLoader.h"
#include "MultiPartImageSequenceScanner.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class MultiPartImageSequenceWidget;
}

class ImageLoaderPlugin;

class MultiPartImageSequenceWidget : public QWidget
{
	Q_OBJECT

public:
	MultiPartImageSequenceWidget(ImageLoaderPlugin* imageLoaderPlugin);

private:
	void onScannerSettingsChanged();
	void onLoaderSettingsChanged();
	void onPickDirectory();
	void onDirectoryChanged(const QString& directory);
	void onLoadPushButtonClicked();
	void onDatasetNameChanged(const QString& dataSetName);
	void onBeginScan();
	void onEndScan(std::shared_ptr<ImageCollections> scanned);
	void onBeginLoad();
	void onEndLoad(std::shared_ptr<Payload> payload);

	void showEvent(QShowEvent* showEvent);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*									_imageLoaderPlugin;
	std::unique_ptr<Ui::MultiPartImageSequenceWidget>	_ui;
	MultiPartImageSequenceScanner						_scanner;
	ImageLoader								_loader;
};