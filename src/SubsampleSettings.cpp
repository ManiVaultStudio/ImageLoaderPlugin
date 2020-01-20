#include "SubsampleSettings.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>

#include <algorithm>

SubsampleSettings::SubsampleSettings(const QString& settingsPath) :
	QObject(),
	Settings("LKEB/CGV", "HDPS", settingsPath),
	_enabled(false),
	_ratio(100.0),
	_filter(ImageResamplingFilter::Bilinear)
{
	_enabled	= setting("enabled", false).toBool();
	_ratio		= setting("ratio", 100.0).toDouble();
	_filter		= ImageResamplingFilter(setting("filter", "bilinear").toInt());

	_filterNames << "Box" << "Bilinear" << "B-spline" << "Bicubic" << "Catmull-Rom" << "Lanczos";

	connect(this, &SubsampleSettings::enabledChanged, this, &SubsampleSettings::settingsChanged);
	connect(this, &SubsampleSettings::ratioChanged, this, &SubsampleSettings::settingsChanged);
	connect(this, &SubsampleSettings::filterChanged, this, &SubsampleSettings::settingsChanged);
}

void SubsampleSettings::loadSettings()
{
	setEnabled(setting("EnableSubsampling", false).toBool(), true);
	setRatio(setting("SubsamplingRatio", 100.0).toDouble(), true);
	setFilter(static_cast<ImageResamplingFilter>(setting("SubsamplingFilter", 0).toInt()), true);
}

bool SubsampleSettings::enabled() const
{
	return _enabled;
}

void SubsampleSettings::setEnabled(const bool& enabled, const bool& notify /*= false*/)
{
	if (!notify && enabled == _enabled)
		return;

	_enabled = enabled;

	setSetting("EnableSubsampling", _enabled);

	qDebug() << "Set image subsampling enabled" << enabled;

	emit enabledChanged(_enabled);
	emit settingsChanged();
}

double SubsampleSettings::ratio() const
{
	return _ratio;
}

void SubsampleSettings::setRatio(const double& resamplingRatio, const bool& notify /*= false*/)
{
	if (!notify && resamplingRatio == _ratio)
		return;

	_ratio = std::max(0.01, std::min(resamplingRatio, 1.0));

	setSetting("SubsamplingRatio", _ratio);

	qDebug() << "Set image subsampling ratio" << resamplingRatio;

	emit ratioChanged(_ratio);
	emit settingsChanged();
}

ImageResamplingFilter SubsampleSettings::filter() const
{
	return _filter;
}

void SubsampleSettings::setFilter(const ImageResamplingFilter& imageResamplingFilter, const bool& notify /*= false*/)
{
	if (!notify && imageResamplingFilter == _filter)
		return;
	
	_filter = imageResamplingFilter;

	const auto filterIndex = static_cast<int>(imageResamplingFilter);

	setSetting("SubsamplingFilter", filterIndex);

	qDebug() << "Set image subsampling filter" << _filterNames.at(filterIndex);

	emit filterChanged(_filter);
	emit settingsChanged();
}

QStringList SubsampleSettings::filterNames() const
{
	return _filterNames;
}