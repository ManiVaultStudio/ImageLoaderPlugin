#pragma once

#include "Common.h"
#include "Settings.h"

#include <QObject>

class ImageLoaderPlugin;

/*!
	\class ColorSettings
	\brief Provides color settings for image loading
	\ingroup ImageLoaderPlugin
*/
class ColorSettings : public QObject, Settings {
	Q_OBJECT

public:
	ColorSettings(const QString& settingsPath);

	void loadSettings();

public:
	bool convertToGrayscale() const;
	void setConvertToGrayscale(const bool& convertToGrayscale, const bool& forceUpdate = false);

signals:
	void convertToGrayscaleChanged(const bool&);
	void settingsChanged();

private:
	bool	_convertToGrayscale;	/*!< Whether to convert to grayscale or not */
};