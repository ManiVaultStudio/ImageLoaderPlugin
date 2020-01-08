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
	QSettings						_settings;							/*!< Settings */
	ImageLoaderPlugin*				_imageLoaderPlugin;					/*!< Image loader plugin */
	QVBoxLayout*					_mainLayout;						/*!< Main UI layout */
	QVBoxLayout*					_settingsLayout;					/*!< Settings layout */
	QComboBox*						_typesComboBox;						/*!< Loader types combobox e.g. sequence, stack */
	StackedWidget*					_pagesStackedWidget;				/*!< Stacked pages widget */
	ImageSequenceWidget*			_imageSequenceWidget;				/*!< Image sequence widget */
	ImageStackWidget*				_imageStackWidget;					/*!< Image stack widget */
	MultiPartImageSequenceWidget*	_multiPartImageSequenceWidget;		/*!< Multi-part image sequence widget */
	QStatusBar*						_statusBar;							/*!< Status bar widget */
};