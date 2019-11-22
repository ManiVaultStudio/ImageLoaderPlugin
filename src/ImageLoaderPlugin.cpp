#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include "ImageData/ImageDataSet.h"
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

	auto& imageDataSet = dynamic_cast<ImageDataSet&>(_core->requestSet(datasetName));
	
	imageDataSet.setRoi(QRect(QPoint(), images.size()));

	ImageData& imageData = dynamic_cast<ImageData&>(imageDataSet.imageData());
	
	switch (images.type()) {
		case ImageCollectionType::Sequence:
		{
			imageData.setSequence(images.images(), images.size());
			break;
		}

		case ImageCollectionType::MultiPartSequence:
		case ImageCollectionType::Stack:
		{
			imageData.setStack(images.images(), images.size());
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