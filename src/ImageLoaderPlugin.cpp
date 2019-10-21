#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include "PointsPlugin.h"
#include "ImageData.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <vector>

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoaderPlugin")

ImageLoaderPlugin::ImageLoaderPlugin() :
	LoaderPlugin("Image Loader")
{
}

void ImageLoaderPlugin::init()
{
}

void ImageLoaderPlugin::loadData()
{
	ImageLoaderDialog dialog(this);
	
	dialog.exec();
}

void ImageLoaderPlugin::addImages(Images& images)
{
	const auto datasetName = _core->addData("Image Data", images.name());

	const auto& dataSet = dynamic_cast<const ImageDataSet&>(_core->requestSet(datasetName));
	
	ImageData& imageData = dynamic_cast<ImageData&>(dataSet.getData());
	
	switch (images.type()) {
		case ImageCollectionType::Sequence:
		{
			imageData.setSequence(images.images(), images.size(), 1);
			break;
		}

		case ImageCollectionType::Stack:
		{
			imageData.setStack(images.images(), images.size(), 1);
			break;
		}
			
		default:
			break;
	}

	_core->notifyDataAdded(datasetName);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}