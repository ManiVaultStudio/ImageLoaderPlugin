#pragma once

#include "ImageCollectionsLoader.h"
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
	void onPickDirectory();
	void onDirectoryChanged(const QString &directory);
	void onLoadPushButtonClicked();
	void onDatasetNameChanged(const QString& text);
	void onBeginScan();
	void onEndScan(ImageCollections& imageCollections);
	void onBeginLoad();
	void onEndLoad(ImageCollections& imageCollections);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	ImageStackScanner						_scanner;
	ImageCollections						_scanned;
	ImageCollectionsLoader					_loader;
};	