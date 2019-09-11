#pragma once

#include "Common.h"
#include "ImageCollections.h"

#include <LoaderPlugin.h>

#include <QSettings>

using hdps::plugin::LoaderPluginFactory;
using hdps::plugin::LoaderPlugin;

class ImageLoaderPlugin : public LoaderPlugin
{
public:
	ImageLoaderPlugin();
    
    void init() override;

    void loadData() Q_DECL_OVERRIDE;

	void addDataSet(ImageCollections& imageCollections);
};

class ImageLoaderPluginFactory : public LoaderPluginFactory
{
	Q_INTERFACES(hdps::plugin::LoaderPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ImageLoaderPlugin"
                      FILE  "ImageLoaderPlugin.json")
    
public:
	ImageLoaderPluginFactory(void) {}
    ~ImageLoaderPluginFactory(void) override {}
    
	LoaderPlugin* produce() override;
};
