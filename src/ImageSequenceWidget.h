#pragma once

#include "ImageSequenceScanner.h"
#include "ImageSequenceLoader.h"

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

	ImageSequenceLoader& loader();

private:
	void onBecameDirty();
	void onBeginScan();
	void onEndScan(QStringList& imageFilePaths);
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);
	void onPickDirectory();
	void onImageTypeChanged(const QString &imageType);
	void onBeginLoad();
	void onEndLoad(FloatVector& pointsData);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*							_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageSequenceWidget>	_ui;
	ImageSequenceScanner						_scanner;
	ImageSequenceLoader							_loader;
};