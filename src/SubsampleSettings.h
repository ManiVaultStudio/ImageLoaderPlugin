#pragma once

#include "Common.h"
#include "Settings.h"

#include <QObject>

class ImageLoaderPlugin;

class SubsampleSettings : public QObject, Settings {
	Q_OBJECT

public:
	 SubsampleSettings(const QString& settingsPath);

	 void loadSettings();

public:
	bool enabled() const;
	void setEnabled(const bool& enabled, const bool& forceUpdate = false);
	double ratio() const;
	void setRatio(const double& resamplingRatio, const bool& forceUpdate = false);
	ImageResamplingFilter filter() const;
	void setFilter(const ImageResamplingFilter& imageResamplingFilter, const bool& forceUpdate = false);
	QStringList filterNames() const;

signals:
	void enabledChanged(const bool&);
	void ratioChanged(const double&);
	void filterChanged(const ImageResamplingFilter&);
	void settingsChanged();

private:
	bool					_enabled;
	double					_ratio;
	ImageResamplingFilter	_filter;
	QStringList				_filterNames;
};