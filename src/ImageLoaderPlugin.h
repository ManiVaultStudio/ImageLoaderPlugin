#pragma once

#include "Common.h"
#include "ResampleImageSettings.h"

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

	void addSequence(const ImageCollectionType& imageCollectionType, const QString &name, const QSize& size, const int& noImages, const int &noDimensions, std::vector<float> &pointsData, const QStringList& dimensionNames = QStringList());

public:
	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant()) const;
	void setSetting(const QString& name, const QVariant& value);

private:
	QSettings				_settings;
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
