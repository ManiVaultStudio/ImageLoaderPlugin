#pragma once

#include "ImageCollectionsLoader.h"
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
	~MultiPartImageSequenceWidget() override;

private:
	void onBecameDirty();
	void onPickDirectory();
	void onDirectoryChanged(const QString& directory);
	void onLoadPushButtonClicked();
	void onDatasetNameChanged(const QString& dataSetName);
	void onBeginScan();
	void onEndScan(const ImageCollections& scannedImageCollections);
	void onBeginLoad();
	void onEndLoad(Payload& payload);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*									_imageLoaderPlugin;
	std::unique_ptr<Ui::MultiPartImageSequenceWidget>	_ui;
	MultiPartImageSequenceScanner						_scanner;
	ImageCollectionsLoader								_loader;
};