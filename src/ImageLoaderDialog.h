#pragma once

#include <memory>

#include <QDialog>
#include <QStackedWidget>

#include "ImageSequence.h"

class QVBoxLayout;
class QComboBox;
class QStackedWidget;

class ImageLoaderPlugin;
class ImageSequenceWidget;
class ImageStackWidget;

class StackedWidget : public QStackedWidget
{
	QSize sizeHint() const
	{
		return currentWidget()->sizeHint();
	}

	QSize minimumSizeHint() const
	{
		return currentWidget()->minimumSizeHint();
	}
};

class ImageLoaderDialog : public QDialog
{
public:
	ImageLoaderDialog(ImageLoaderPlugin* imageLoaderPlugin);
	~ImageLoaderDialog();

private:
	ImageLoaderPlugin*						_imageLoaderPlugin;
	std::unique_ptr<QVBoxLayout>			_mainLayout;
	std::unique_ptr<QComboBox>				_typesComboBox;
	std::unique_ptr<StackedWidget>			_pagesStackedWidget;
	std::unique_ptr<ImageSequenceWidget>	_imageSequenceWidget;
	std::unique_ptr<ImageStackWidget>		_imageStackWidget;
};