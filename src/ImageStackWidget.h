#pragma once

#include "ImageStackLoader.h"
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
	void onLoadSequence();
	void onBeginScan();
	void onEndScan(QMap<QString, QStringList>& imageStacks);
	void onBeginLoad();
	void onEndLoad(FloatVector& pointsData);

signals:
	void message(const QString& message);

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;
	std::unique_ptr<Ui::ImageStackWidget>	_ui;
	ImageStackScanner						_scanner;
	ImageStackLoader						_loader;
};	