#pragma once

#include "Common.h"

#include <QObject>
#include <QSettings>

class ImageLoaderPlugin;

class ColorSettings : public QObject {
	Q_OBJECT

public:
	ColorSettings(QSettings* settings);
	 ~ColorSettings() override;

	 void emitAll();

public:
	bool convertToGrayscale() const;
	void setConvertToGrayscale(const bool& convertToGrayscale);

private:
	QString settingPath(const QString& name) const;

signals:
	void convertToGrayscaleChanged(const bool&);
	void settingsChanged();

private:
	QSettings*	_settings;
	bool		_convertToGrayscale;
};