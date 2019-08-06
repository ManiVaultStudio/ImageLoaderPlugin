#pragma once
#include <LoaderPlugin.h>

using hdps::plugin::LoaderPluginFactory;
using hdps::plugin::LoaderPlugin;

class ImageLoaderPlugin : public LoaderPlugin
{
public:
	ImageLoaderPlugin() : LoaderPlugin("Image Loader") { }
    ~ImageLoaderPlugin(void);
    
    void init();

    void loadData() Q_DECL_OVERRIDE;

	void addSequence(const QString &name, const int &noDimensions, std::vector<float> &pointsData);
};

class ImageLoaderPluginFactory : public LoaderPluginFactory
{
	Q_INTERFACES(hdps::plugin::LoaderPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ImageLoaderPlugin"
                      FILE  "ImageLoaderPlugin.json")
    
public:
	ImageLoaderPluginFactory(void) {}
    ~ImageLoaderPluginFactory(void) {}
    
	LoaderPlugin* produce();
};
