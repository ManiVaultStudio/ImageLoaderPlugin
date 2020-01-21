#pragma once

#include "Common.h"
#include "Settings.h"

/**
 * Subsample settings class
 * This class provide settings for image subsampling
 * @author Thomas Kroes
 */
class SubsampleSettings : public QObject, Settings {
	Q_OBJECT

public:
	/**
	 * Constructor
	 * @param settingsPath Settings path prefix
	 */
	 SubsampleSettings(const QString& settingsPath);

	 /** Loads settings */
	 void loadSettings();

public:
	/** Returns whether subsampling is enabled */
	bool enabled() const;

	/**
	 * Sets whether subsampling is enabled
	 * @param enabled Whether subsampling is enabled
	 * @param notify Whether to notify others of this change
	 */
	void setEnabled(const bool& enabled, const bool& notify = false);

	/** Returns the subsampling ratio [0.01...1.0] */
	double ratio() const;

	/**
	 * Sets the subsampling ratio [0.01...1.0]
	 * @param resamplingRatio Ratio at which to subsample the image [0.01...1.0]
	 * @param notify Whether to notify others of this change
	 */
	void setRatio(const double& resamplingRatio, const bool& notify = false);

	/** Returns the image resampling filter type */
	ImageResamplingFilter filter() const;

	/**
	 * Sets the image resampling filter type
	 * @param imageResamplingFilter Type of image resampling
	 * @param notify Whether to notify others of this change
	 */
	void setFilter(const ImageResamplingFilter& imageResamplingFilter, const bool& notify = false);

	/** Returns the names of available filters */
	QStringList filterNames() const;

signals:
	/**
	 * Signals that the enabled property has changed
	 * @param enabled Subsampling enabled on/off
	 */
	void enabledChanged(const bool& enabled);

	/**
	 * Signals that the subsampling ratio has changed
	 * @param ratio Subsampling ratio
	 */
	void ratioChanged(const double& ratio);

	/**
	 * Signals that the subsampling filter has changed
	 * @param imageResamplingFilter Subsampling filter
	 */
	void filterChanged(const ImageResamplingFilter& imageResamplingFilter);

	/** Signals that the subsampling settings have changed */
	void settingsChanged();

private:
	bool					_enabled;			/** Whether subsampling is enabled or not */
	double					_ratio;				/** Ratio of subsampling */
	ImageResamplingFilter	_filter;			/** Subsampling filter e.g. box, bilinear */
	QStringList				_filterNames;		/** Subsampling filter names */
};