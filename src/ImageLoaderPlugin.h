#pragma once

#include "Common.h"
#include "ImageCollectionScanner.h"
#include "ImageCollectionsModel.h"
#include "ConversionAction.h"

#include <LoaderPlugin.h>
#include <PluginFactory.h>

using mv::plugin::LoaderPluginFactory;
using mv::plugin::LoaderPlugin;

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
    ImageLoaderPlugin(const mv::plugin::PluginFactory* factory);

    ~ImageLoaderPlugin() override;

    /** Initializes the plugin */
    void init() override {};

    /** Load high dimensional image data */
    void loadData() Q_DECL_OVERRIDE;

    ImageCollectionScanner& getImageCollectionScanner() { return _imageCollectionScanner; }
    ImageCollectionsModel& getImageCollectionsModel() { return _imageCollectionsModel; }
    ImageCollectionsModel::Filter& getImageCollectionsFilterModel() { return _imageCollectionsFilterModel; }
    mv::gui::PluginTriggerPickerAction& getConversionPickerAction() { return _conversionPickerAction; }

    QModelIndexList getSelectedRows() const;;

private:
    ImageCollectionScanner              _imageCollectionScanner;        /** Image collection scanner */
    ImageCollectionsModel               _imageCollectionsModel;         /** Image collections model */
    ImageCollectionsModel::Filter       _imageCollectionsFilterModel;   /** Image collections filter model */
    ConversionAction                    _conversionPickerAction;        /** Conversion plugin trigger picker action */

    friend class ImageCollection;
};

/**
 * Image loader plugin factory class
 * A factory for creating image loader plugin instances
 */
class ImageLoaderPluginFactory : public LoaderPluginFactory
{
    Q_INTERFACES(mv::plugin::LoaderPluginFactory mv::plugin::PluginFactory)
        Q_OBJECT
        Q_PLUGIN_METADATA(IID   "studio.manivault.ImageLoaderPlugin" 
                          FILE  "PluginInfo.json")

public:
    /** Default constructor */
    ImageLoaderPluginFactory();

    /** Destructor */
    ~ImageLoaderPluginFactory() override {}

    /**
     * Produces the plugin
     * @return Pointer to the produced plugin
     */
    LoaderPlugin* produce() override;
};
