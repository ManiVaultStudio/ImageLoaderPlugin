#pragma once

#include "MultiPartImageSequenceScanner.h"
#include "MultiPartImageSequenceLoader.h"

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
	void onBeginScan();
	void onEndScan();
	void onBeginLoad();
	void onEndLoad();

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*									_imageLoaderPlugin;
	std::unique_ptr<Ui::MultiPartImageSequenceWidget>	_ui;
	MultiPartImageSequenceScanner						_scanner;
	MultiPartImageSequenceLoader						_loader;
};