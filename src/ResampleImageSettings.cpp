#include "ResampleImageSettings.h"
#include "ImageUtilities.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

ResampleImageSettings::ResampleImageSettings(QSettings* settings) :
	_settings(settings),
	_ratio(100.0),
	_filter(ImageResamplingFilter::Bilinear)
{
	_ratio	= _settings->value(settingPath("ratio"), 100.0).toDouble();
	_filter = ImageResamplingFilter(_settings->value(settingPath("filter"), "bilinear").toInt());

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

	emit ratioChanged(_ratio);

	_settings->setValue(settingPath("ratio"), _ratio);
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

	emit filterChanged(_filter);

	_settings->setValue(settingPath("filter"), filterIndex);
}

QStringList ResampleImageSettings::filterNames() const
{
	return _filterNames;
}

QString ResampleImageSettings::settingPath(const QString& name) const
{
	return QString("Resampling/%2").arg(name);
}
