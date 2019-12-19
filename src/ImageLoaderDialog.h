#pragma once

#include <QDialog>
#include <QSettings>
#include <QStackedWidget>
#include <QLabel>

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
private:
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
	void onMessage(const QString& message);

private:
	QSettings						_settings;
	ImageLoaderPlugin*				_imageLoaderPlugin;
	QVBoxLayout*					_mainLayout;
	QVBoxLayout*					_settingsLayout;
	QComboBox*						_typesComboBox;
	StackedWidget*					_pagesStackedWidget;
	ImageSequenceWidget*			_imageSequenceWidget;
	ImageStackWidget*				_imageStackWidget;
	MultiPartImageSequenceWidget*	_multiPartImageSequenceWidget;
	QStatusBar*						_statusBar;
};