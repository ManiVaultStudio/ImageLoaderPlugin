#include "SubsampleImageSettings.h"
#include "ImageUtilities.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

SubsampleImageSettings::SubsampleImageSettings(QSettings* settings) :
	_settings(settings),
	_enabled(false),
	_ratio(100.0),
	_filter(ImageResamplingFilter::Bilinear)
{
	_enabled	= _settings->value(settingPath("enabled"), false).toBool();
	_ratio		= _settings->value(settingPath("ratio"), 100.0).toDouble();
	_filter		= ImageResamplingFilter(_settings->value(settingPath("filter"), "bilinear").toInt());

	_filterNames << "Box" << "Bilinear" << "B-spline" << "Bicubic" << "Catmull-Rom" << "Lanczos";
}

SubsampleImageSettings::~SubsampleImageSettings()
{
	_settings->setValue(settingPath("enabled"), _enabled);
	_settings->setValue(settingPath("ratio"), _ratio);
	_settings->setValue(settingPath("filter"), static_cast<int>(_filter));
}

void SubsampleImageSettings::emitAll()
{
	emit enabledChanged(_enabled);
	emit ratioChanged(_ratio);
	emit filterChanged(_filter);
}

bool SubsampleImageSettings::enabled() const
{
	return _enabled;
}

void SubsampleImageSettings::setEnabled(const bool& enabled)
{
	if (enabled == _enabled)
		return;

	qDebug() << "Image resampling enabled changed to" << enabled;

	_enabled = enabled;

	emit enabledChanged(_enabled);
}

double SubsampleImageSettings::ratio() const
{
	return _ratio;
}

void SubsampleImageSettings::setRatio(const double& ratio)
{
	if (ratio == _ratio)
		return;

	qDebug() << "Image resamping ratio changed to" << ratio;

	_ratio = ratio;

	emit ratioChanged(_ratio);
}

ImageResamplingFilter SubsampleImageSettings::filter() const
{
	return _filter;
}

void SubsampleImageSettings::setFilter(const ImageResamplingFilter& filter)
{
	if (filter == _filter)
		return;
	
	const auto filterIndex = static_cast<int>(filter);

	qDebug() << "Image resamping filter changed to" << _filterNames.at(filterIndex);

	_filter = filter;

	emit filterChanged(_filter);
}

QStringList SubsampleImageSettings::filterNames() const
{
	return _filterNames;
}

QString SubsampleImageSettings::settingPath(const QString& name) const
{
	return QString("Resampling/%2").arg(name);
}
