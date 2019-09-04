#include "ResampleImageSettings.h"
#include "ImageUtilities.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

ResampleImageSettings::ResampleImageSettings() :
	_imageLoaderPlugin(nullptr),
	_category(""),
	_ratio(100.0),
	_filter(ImageResamplingFilter::Bilinear)
{
	_filterNames << "Box" << "Bilinear" << "B-spline" << "Bicubic" << "Catmull-Rom" << "Lanczos";
}

void ResampleImageSettings::initialize(ImageLoaderPlugin* imageLoaderPlugin, const QString& category)
{
	_imageLoaderPlugin	= imageLoaderPlugin;
	_category			= category;
	_ratio				= _imageLoaderPlugin->setting(settingPath("ratio"), 100.0).toDouble();
	_filter				= ImageResamplingFilter(_imageLoaderPlugin->setting(settingPath("filter"), "bilinear").toInt());
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

	_imageLoaderPlugin->setSetting(settingPath("ratio"), _ratio);
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

	_imageLoaderPlugin->setSetting(settingPath("filter"), filterIndex);
}

QStringList ResampleImageSettings::filterNames() const
{
	return _filterNames;
}

QString ResampleImageSettings::settingPath(const QString& name) const
{
	return QString("%1/resampling/%2").arg(_category, name);
}
