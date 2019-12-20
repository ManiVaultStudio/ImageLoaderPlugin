#include "Settings.h"

Settings::Settings(const QString& organization, const QString& application, const QString& prefix) :
	_settings(organization, application),
	_prefix(prefix)
{
}

QString Settings::prefix() const
{
	return _prefix;
}

QVariant Settings::setting(const QString& name, const QVariant& defaultValue /*= QVariant()*/)
{
	return _settings.value(QString("%1/%2").arg(_prefix, name), defaultValue);
}

void Settings::setSetting(const QString& name, const QVariant& value)
{
	_settings.setValue(QString("%1/%2").arg(_prefix, name), value);
}