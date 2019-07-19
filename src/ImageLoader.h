#pragma once
#include <LoaderPlugin.h>

using hdps::plugin::LoaderPluginFactory;
using hdps::plugin::LoaderPlugin;

class ImageLoader : public LoaderPlugin
{
public:
	ImageLoader() : LoaderPlugin("Image Loader") { }
    ~ImageLoader(void);
    
    void init();

    void loadData() Q_DECL_OVERRIDE;

	void addSequence(const QString &name, const int &noDimensions, const std::vector<float> &pointsData);
};

class ImageLoaderFactory : public LoaderPluginFactory
{
	Q_INTERFACES(hdps::plugin::LoaderPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "nl.tudelft.ImageLoader"
                      FILE  "ImageLoader.json")
    
public:
	ImageLoaderFactory(void) {}
    ~ImageLoaderFactory(void) {}
    
	LoaderPlugin* produce();
};
