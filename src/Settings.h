#pragma once

#include <QSettings>

class Settings
{
public:
	Settings(const QString& organization, const QString& application, const QString& prefix);

	QString prefix() const;

	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant());
	void setSetting(const QString& name, const QVariant& value);

private:
	QSettings	_settings;
	QString		_prefix;
};