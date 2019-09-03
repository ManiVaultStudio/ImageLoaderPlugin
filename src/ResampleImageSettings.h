#pragma once

#include "Common.h"

#include <QObject>

class ImageLoaderPlugin;

class ResampleImageSettings : public QObject {
	Q_OBJECT

public:
	 ResampleImageSettings(ImageLoaderPlugin* imageLoaderPlugin);

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
	ImageLoaderPlugin*		_imageLoaderPlugin;
	double					_ratio;
	ImageResamplingFilter	_filter;
	QStringList				_filterNames;
};