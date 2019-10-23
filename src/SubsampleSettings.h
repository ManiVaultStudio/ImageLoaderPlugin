#pragma once

#include "Common.h"

#include <QObject>
#include <QSettings>

class ImageLoaderPlugin;

class SubsampleSettings : public QObject {
	Q_OBJECT

public:
	 SubsampleSettings(QSettings* settings);
	 ~SubsampleSettings() override;

	 void emitAll();

public:
	bool enabled() const;
	void setEnabled(const bool& enabled);
	double ratio() const;
	void setRatio(const double& resamplingRatio);
	ImageResamplingFilter filter() const;
	void setFilter(const ImageResamplingFilter& imageResamplingFilter);
	QStringList filterNames() const;

private:
	QString settingPath(const QString& name) const;

signals:
	void enabledChanged(const bool&);
	void ratioChanged(const double&);
	void filterChanged(const ImageResamplingFilter&);
	void changed();

private:
	QSettings*				_settings;
	bool					_enabled;
	double					_ratio;
	ImageResamplingFilter	_filter;
	QStringList				_filterNames;
};