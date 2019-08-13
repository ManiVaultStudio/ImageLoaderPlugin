#pragma once

#include <LoaderPlugin.h>

#include <QSettings>

using hdps::plugin::LoaderPluginFactory;
using hdps::plugin::LoaderPlugin;

class ImageLoaderPlugin : public LoaderPlugin
{
public:
	ImageLoaderPlugin();
    ~ImageLoaderPlugin(void) override;
    
    void init() override;

    void loadData() Q_DECL_OVERRIDE;

	enum ImageCollectionType
	{
		Sequence,
		Stack
	};

	void addSequence(const ImageCollectionType& imageCollectionType, const QString &name, const QSize& size, const int& noImages, const int &noDimensions, std::vector<float> &pointsData);

	QSettings _settings;
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
