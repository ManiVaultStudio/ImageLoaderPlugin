#pragma once

#include "Common.h"

#include "ImageCollectionsModel.h"

#include <LoaderPlugin.h>

using hdps::plugin::LoaderPluginFactory;
using hdps::plugin::LoaderPlugin;

/**
 * Image loader plugin class
 *
 * This image loader plugin class provides functionality to load high-dimensional image data into HDPS
 *
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

public: // Models

	/** Returns the image collections model */
	ImageCollectionsModel& getImageCollectionsModel() { return _imageCollectionsModel; }

	/** Returns the image collections model */
	ImageCollectionsModel::Filter& getImageCollectionsFilterModel() { return _imageCollectionsFilterModel; }

private:
	ImageCollectionsModel			_imageCollectionsModel;				/** Image collections model */
	ImageCollectionsModel::Filter	_imageCollectionsFilterModel;		/** Image collections filter model */

	friend class ImageCollection;
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