#include "SubsampleSettings.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

SubsampleSettings::SubsampleSettings(QSettings* settings) :
	_settings(settings),
	_enabled(false),
	_ratio(100.0),
	_filter(ImageResamplingFilter::Bilinear)
{
	_enabled	= _settings->value(settingPath("enabled"), false).toBool();
	_ratio		= _settings->value(settingPath("ratio"), 100.0).toDouble();
	_filter		= ImageResamplingFilter(_settings->value(settingPath("filter"), "bilinear").toInt());

	_filterNames << "Box" << "Bilinear" << "B-spline" << "Bicubic" << "Catmull-Rom" << "Lanczos";

	connect(this, &SubsampleSettings::enabledChanged, this, &SubsampleSettings::settingsChanged);
	connect(this, &SubsampleSettings::ratioChanged, this, &SubsampleSettings::settingsChanged);
	connect(this, &SubsampleSettings::filterChanged, this, &SubsampleSettings::settingsChanged);
}

SubsampleSettings::~SubsampleSettings()
{
	_settings->setValue(settingPath("enabled"), _enabled);
	_settings->setValue(settingPath("ratio"), _ratio);
	_settings->setValue(settingPath("filter"), static_cast<int>(_filter));
}

void SubsampleSettings::emitAll()
{
	emit enabledChanged(_enabled);
	emit ratioChanged(_ratio);
	emit filterChanged(_filter);
}

bool SubsampleSettings::enabled() const
{
	return _enabled;
}

void SubsampleSettings::setEnabled(const bool& enabled)
{
	if (enabled == _enabled)
		return;

	qDebug() << "Image resampling enabled changed to" << enabled;

	_enabled = enabled;

	emit enabledChanged(_enabled);
	emit settingsChanged();
}

double SubsampleSettings::ratio() const
{
	return _ratio;
}

void SubsampleSettings::setRatio(const double& ratio)
{
	if (ratio == _ratio)
		return;

	qDebug() << "Image resampling ratio changed to" << ratio;

	_ratio = ratio;

	emit ratioChanged(_ratio);
	emit settingsChanged();
}

ImageResamplingFilter SubsampleSettings::filter() const
{
	return _filter;
}

void SubsampleSettings::setFilter(const ImageResamplingFilter& filter)
{
	if (filter == _filter)
		return;
	
	const auto filterIndex = static_cast<int>(filter);

	qDebug() << "Image resampling filter changed to" << _filterNames.at(filterIndex);

	_filter = filter;

	emit filterChanged(_filter);
	emit settingsChanged();
}

QStringList SubsampleSettings::filterNames() const
{
	return _filterNames;
}

QString SubsampleSettings::settingPath(const QString& name) const
{
	return QString("Resampling/%2").arg(name);
}