#pragma once

#include <QSettings>

/**
 * Settings class
 * Convenience class for serializing persistent image loader/scanner and UI parameters
 * @author Thomas Kroes
 */
class Settings
{
public:
	/**
	 * Constructor
	 * @param organization Organization name
	 * @param application Application name
	 * @param prefix Prefix name
	 */
	Settings(const QString& organization, const QString& application, const QString& prefix);

	/** Returns the prefix */
	QString prefix() const;

	/**
	 * Retrieves a setting
	 * @param name Name of the setting
	 * @param defaultValue Default value of the setting
	 */
	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant());

	/**
	 * Sets a setting
	 * @param name Name of the setting
	 * @param value Setting value
	 */
	void setSetting(const QString& name, const QVariant& value);

private:
	QSettings	_settings;		/** Wrapped QSettings object */
	QString		_prefix;		/** Settings prefix */
};