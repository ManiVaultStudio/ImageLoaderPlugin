#pragma once

#include "Common.h"

#include <QObject>
#include <QSettings>

class ImageLoaderPlugin;

class ResampleImageSettings : public QObject {
	Q_OBJECT

public:
	 ResampleImageSettings(QSettings* settings);

signals:
	void resamplingRatioChanged(const double&);
	void imageResamplingFilterChanged(const ImageResamplingFilter&);

public:
	double ratio() const;
	void setRatio(const double& resamplingRatio);
	ImageResamplingFilter filter() const;
	void setFilter(const ImageResamplingFilter& imageResamplingFilter);
	QStringList filterNames() const;

private:
	QString settingPath(const QString& name) const;

private:
	QSettings*				_settings;
	double					_ratio;
	ImageResamplingFilter	_filter;
	QStringList				_filterNames;
};