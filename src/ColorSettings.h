#pragma once

#include "Common.h"
#include "Settings.h"

/**
 * ColorSettings
 * Provides color settings for image loading
 */
class ColorSettings : public QObject, Settings {
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param settingsPath Settings path
	 */
	ColorSettings(const QString& settingsPath);

	/** Loads settings */
	void loadSettings();

public:
	/** Returns whether images should be converted to grayscale */
	bool convertToGrayscale() const;

	/**
	 * Sets whether images should be converted to grayscale
	 * @param convertToGrayscale Whether images should be converted to grayscale
	 * @param notify Whether to notify others of this change
	 */
	void setConvertToGrayscale(const bool& convertToGrayscale, const bool& notify = false);

signals:
	/**
	 * Signals that the convert to grayscale property has changed
	 * @param convertToGrayscale Whether images should be converted to grayscale
	 */
	void convertToGrayscaleChanged(const bool& convertToGrayscale);

	/** Signals that the color settings have changed */
	void settingsChanged();

private:
	bool	_convertToGrayscale;	/** Whether to convert to grayscale or not */
};