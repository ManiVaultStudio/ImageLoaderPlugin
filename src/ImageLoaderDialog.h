#pragma once

#include <QDialog>
#include <QSettings>
#include <QStackedWidget>
#include <QLabel>

#include <memory>

class QVBoxLayout;
class QComboBox;
class QStackedWidget;
class QStatusBar;
class QProgressBar;

class ImageLoaderPlugin;
class CommonSettingsWidget;
class StackSettingsWidget;
class MultiPartImageSequenceWidget;
class ResampleImageSettingsWidget;

namespace Ui {
	class ImageLoaderDialog;
}

/**
 * Image loader dialog class
 * This class provides a user interface for loading high-dimensional image data
 */
class ImageLoaderDialog : public QDialog
{
public:
	/**
	 * Constructor
	 * @param parent Parent object
	 */
	ImageLoaderDialog(QObject* parent = nullptr);

	/** Destructor */
	~ImageLoaderDialog() override;

	/**
	 * Initializes the dialog
	 * @param imageLoaderPlugin Pointer to image loader plugin
	 */
	void initialize(ImageLoaderPlugin* imageLoaderPlugin);

private:
	std::unique_ptr<Ui::ImageLoaderDialog>		_ui;						/** Externally loaded UI */
	QSettings									_settings;					/** Settings */
	ImageLoaderPlugin*							_imageLoaderPlugin;			/** Image loader plugin */
};