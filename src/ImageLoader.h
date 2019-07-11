#pragma once

#include <LoaderPlugin.h>

using hdps::plugin::LoaderPluginFactory;
using hdps::plugin::LoaderPlugin;
//using namespace hdps::plugin;

// =============================================================================
// View
// =============================================================================

class ImageLoader : public LoaderPlugin
{
public:
	ImageLoader() : LoaderPlugin("Image Loader") { }
    ~ImageLoader(void);
    
    void init();

    void loadData() Q_DECL_OVERRIDE;
};


// =============================================================================
// Factory
// =============================================================================

class ImageLoaderFactory : public LoaderPluginFactory
{
	Q_INTERFACES(hdps::plugin::LoaderPluginFactory hdps::plugin::LoaderPluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ImageLoader"
                      FILE  "ImageLoader.json")
    
public:
	ImageLoaderFactory(void) {}
    ~ImageLoaderFactory(void) {}
    
	LoaderPlugin* produce();
};
