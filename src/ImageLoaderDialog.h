#pragma once

#include <memory>

#include <QDialog>
#include <QStackedWidget>

#include "ImageSequence.h"

class QVBoxLayout;
class QComboBox;
class QStackedWidget;
class QStatusBar;

class ImageLoaderPlugin;
class ImageSequenceWidget;
class ImageStackWidget;
class MultiPartImageSequenceWidget;
class ResampleImageSettingsWidget;

class StackedWidget : public QStackedWidget
{
	QSize sizeHint() const override
	{
		return currentWidget()->sizeHint();
	}

	QSize minimumSizeHint() const override
	{
		return currentWidget()->minimumSizeHint();
	}
};

class ImageLoaderDialog : public QDialog
{
public:
	ImageLoaderDialog(ImageLoaderPlugin* imageLoaderPlugin);
	~ImageLoaderDialog() override;

private:
	ImageLoaderPlugin*								_imageLoaderPlugin;
	std::unique_ptr<QVBoxLayout>					_mainLayout;
	std::unique_ptr<QComboBox>						_typesComboBox;
	std::unique_ptr<StackedWidget>					_pagesStackedWidget;
	std::unique_ptr<ImageSequenceWidget>			_imageSequenceWidget;
	std::unique_ptr<ImageStackWidget>				_imageStackWidget;
	std::unique_ptr<MultiPartImageSequenceWidget>	_multiPartImageSequenceWidget;
	//std::unique_ptr<ResampleImageSettingsWidget>	_resampleWidget;
	std::unique_ptr<QStatusBar>						_statusBar;
};