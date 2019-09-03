#pragma once

#include "ResampleImageSettings.h"

#include <memory>

#include <QWidget>

namespace Ui {
	class ResampleImageSettingsWidget;
}

class ImageLoaderPlugin;

class ResampleImageSettingsWidget : public QWidget
{
public:
	ResampleImageSettingsWidget(ImageLoaderPlugin* imageLoaderPlugin);
	~ResampleImageSettingsWidget() override;

private:
	ResampleImageSettings& resampleImageSettings();

private:
	ImageLoaderPlugin*									_imageLoaderPlugin;
	std::unique_ptr<Ui::ResampleImageSettingsWidget>	_ui;
	//ImageStacks								_imageStacks;
};	