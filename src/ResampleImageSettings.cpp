#include "ResampleImageSettings.h"
#include "ImageUtilities.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

ResampleImageSettings::ResampleImageSettings(ImageLoaderPlugin* imageLoaderPlugin) :
	_imageLoaderPlugin(imageLoaderPlugin),
	_ratio(100.0),
	_filter(ImageResamplingFilter::Bilinear)
{
	_ratio	= _imageLoaderPlugin->setting("multipart/resampling/ratio", 100.0).toDouble();
	_filter	= ImageResamplingFilter(_imageLoaderPlugin->setting("multipart/resampling/filter", "bilinear").toInt());

	_filterNames << "Box" << "Bilinear" << "B-spline" << "Bicubic" << "Catmull-Rom" << "Lanczos";
}

double ResampleImageSettings::ratio() const
{
	return _ratio;
}

void ResampleImageSettings::setRatio(const double& ratio)
{
	qDebug() << "Image resamping ratio changed to" << ratio;

	_ratio = ratio;

	emit resamplingRatioChanged(_ratio);

	_imageLoaderPlugin->setSetting("multipart/resampling/ratio", _ratio);
}

ImageResamplingFilter ResampleImageSettings::filter() const
{
	return _filter;
}

void ResampleImageSettings::setFilter(const ImageResamplingFilter& filter)
{
	const auto filterIndex = static_cast<int>(filter);

	qDebug() << "Image resamping filter changed to" << _filterNames.at(filterIndex);

	_filter = filter;

	emit imageResamplingFilterChanged(_filter);

	_imageLoaderPlugin->setSetting("multipart/resampling/filter", filterIndex);
}

QStringList ResampleImageSettings::filterNames() const
{
	return _filterNames;
}
