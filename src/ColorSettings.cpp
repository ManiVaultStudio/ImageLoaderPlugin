#include "ColorSettings.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

ColorSettings::ColorSettings(QSettings* settings) :
	_settings(settings),
	_convertToGrayscale(false)
{
	_convertToGrayscale = _settings->value(settingPath("convertToGrayscale"), false).toBool();

	connect(this, &ColorSettings::convertToGrayscaleChanged, this, &ColorSettings::changed);
}

ColorSettings::~ColorSettings()
{
	_settings->setValue(settingPath("convertToGrayscale"), _convertToGrayscale);
}

void ColorSettings::emitAll()
{
	emit convertToGrayscaleChanged(_convertToGrayscale);
}

bool ColorSettings::convertToGrayscale() const
{
	return _convertToGrayscale;
}

void ColorSettings::setConvertToGrayscale(const bool& convertToGrayscale)
{
	if (convertToGrayscale == _convertToGrayscale)
		return;

	_convertToGrayscale  = convertToGrayscale;

	qDebug() << "Convert to gray scale set to" << _convertToGrayscale;

	emit convertToGrayscaleChanged(_convertToGrayscale);
}

QString ColorSettings::settingPath(const QString& name) const
{
	return QString("Resampling/%2").arg(name);
}
