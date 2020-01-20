#include "ColorSettings.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

ColorSettings::ColorSettings(const QString& settingsPath) :
	QObject(),
	Settings("LKEB/CGV", "HDPS", settingsPath),
	_convertToGrayscale(false)
{
	_convertToGrayscale = setting("convertToGrayscale", false).toBool();

	connect(this, &ColorSettings::convertToGrayscaleChanged, this, &ColorSettings::settingsChanged);
}

void ColorSettings::loadSettings()
{
	setConvertToGrayscale(setting("ConvertToGrayscale", false).toBool(), true);
}

bool ColorSettings::convertToGrayscale() const
{
	return _convertToGrayscale;
}

void ColorSettings::setConvertToGrayscale(const bool& convertToGrayscale, const bool& notify /*= false*/)
{
	if (!notify && convertToGrayscale == _convertToGrayscale)
		return;

	_convertToGrayscale = convertToGrayscale;

	setSetting("ConvertToGrayscale", _convertToGrayscale);

	qDebug() << "Set convert to grayscale" << convertToGrayscale;

	emit convertToGrayscaleChanged(_convertToGrayscale);
	emit settingsChanged();
}