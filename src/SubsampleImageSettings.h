#pragma once

#include "Common.h"

#include <QObject>
#include <QSettings>

class ImageLoaderPlugin;

class SubsampleImageSettings : public QObject {
	Q_OBJECT

public:
	 SubsampleImageSettings(QSettings* settings);
	 ~SubsampleImageSettings();

signals:
	void enabledChanged(const bool&);
	void ratioChanged(const double&);
	void filterChanged(const ImageResamplingFilter&);

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

private:
	QSettings*				_settings;
	bool					_enabled;
	double					_ratio;
	ImageResamplingFilter	_filter;
	QStringList				_filterNames;
};