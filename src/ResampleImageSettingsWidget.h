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
	ResampleImageSettingsWidget(QWidget* parent);
	~ResampleImageSettingsWidget() override;

	void initialize(ImageLoaderPlugin* imageLoaderPlugin);

private:
	ImageLoaderPlugin*									_imageLoaderPlugin;
	std::unique_ptr<Ui::ResampleImageSettingsWidget>	_ui;
};	