#pragma once

#include "Common.h"
#include "Payload.h"

#include "ImageCollectionsModel.h"

#include <LoaderPlugin.h>

#include <QSettings>

using hdps::plugin::LoaderPluginFactory;
using hdps::plugin::LoaderPlugin;

/**
 * Image loader plugin class
 * This loader plugin class provides functionality to load high-dimensional image data into HDPS
 * @author Thomas Kroes
 */
class ImageLoaderPlugin : public LoaderPlugin
{
public:
	/** Default constructor */
	ImageLoaderPlugin();
    
	/** Initializes the plugin */
    void init() override;

	/** Load high dimensional image data */
    void loadData() Q_DECL_OVERRIDE;

	/**
	 * Add images to HDPS
	 * @param payload High dimensional image data payload
	 */
	QString addImages(std::shared_ptr<Payload> payload);

public: // Models

	/** Returns the image collections model */
	ImageCollectionsModel& imageCollectionsModel() { return _imageCollectionsModel; }

	/** Returns the image collections model */
	ImageCollectionsModel::Filter& imageCollectionsFilterModel() { return _imageCollectionsFilterModel; }

private:
	ImageCollectionsModel			_imageCollectionsModel;				/** Image collections model */
	ImageCollectionsModel::Filter	_imageCollectionsFilterModel;		/** Image collections filter model */
};

/**
 * Image loader plugin factory class
 * A factory for creating image loader plugin instances
 */
class ImageLoaderPluginFactory : public LoaderPluginFactory
{
	Q_INTERFACES(hdps::plugin::LoaderPluginFactory hdps::plugin::PluginFactory)
		Q_OBJECT
		Q_PLUGIN_METADATA(IID   "nl.tudelft.ImageLoaderPlugin" FILE  "ImageLoaderPlugin.json")

public:
	/** Default constructor */
	ImageLoaderPluginFactory(void) {}

	/** Destructor */
	~ImageLoaderPluginFactory(void) override {}

	/** Creates an image loader plugin instance */
	LoaderPlugin* produce() override;
};