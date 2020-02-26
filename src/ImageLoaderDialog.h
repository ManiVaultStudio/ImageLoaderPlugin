#pragma once

#include "ImageLoader.h"

#include <QDialog>
#include <QSettings>
#include <QStackedWidget>
#include <QLabel>

class QVBoxLayout;
class QComboBox;
class QStackedWidget;
class QStatusBar;
class QProgressBar;

class ImageLoaderPlugin;
class ImageSequenceWidget;
class ImageStackWidget;
class MultiPartImageSequenceWidget;
class ResampleImageSettingsWidget;

/**
 * Stacked widget class
 * Helper class to ensure size hint
 * @author Thomas Kroes
 */
class StackedWidget : public QStackedWidget
{
private:
	/** Returns the hacked size hint */
	QSize sizeHint() const override
	{
		return currentWidget()->sizeHint();
	}

	/** Returns the hacked minimum size hint */
	QSize minimumSizeHint() const override
	{
		return currentWidget()->minimumSizeHint();
	}
};

/**
 * Image loader dialog class
 * This class provides a user interface for loading high-dimensional image data
 */
class ImageLoaderDialog : public QDialog
{
public:
	/**
	 * Constructor
	 * @param imageLoaderPlugin Pointer to image loader plugin class
	 */
	ImageLoaderDialog(ImageLoaderPlugin* imageLoaderPlugin);

	/** Destructor */
	~ImageLoaderDialog() override;

private:
	/**
	 * Invoked when a message is received
	 * @param message Received message
	 */
	void onMessage(const QString& message);

private:
	QSettings						_settings;							/** Settings */
	ImageLoaderPlugin*				_imageLoaderPlugin;					/** Image loader plugin */
	QVBoxLayout*					_mainLayout;						/** Main UI layout */
	QVBoxLayout*					_settingsLayout;					/** Settings layout */
	QComboBox*						_typesComboBox;						/** Loader types combobox e.g. sequence, stack */
	StackedWidget*					_pagesStackedWidget;				/** Stacked pages widget */
	ImageSequenceWidget*			_imageSequenceWidget;				/** Image sequence widget */
	ImageStackWidget*				_imageStackWidget;					/** Image stack widget */
	MultiPartImageSequenceWidget*	_multiPartImageSequenceWidget;		/** Multi-part image sequence widget */
	QStatusBar*						_statusBar;							/** Status bar widget */
	QProgressBar*					_progressBar;						/** Progress bar widget */
};