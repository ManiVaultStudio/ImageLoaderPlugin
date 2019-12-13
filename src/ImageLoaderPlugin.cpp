#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include "ImageData/Images.h"
#include "Set.h"

#include <QtCore>
#include <QDebug>

#include "PointData.h"

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

void ImageLoaderPlugin::addImages(Payload& payload)
{
	const auto datasetName = _core->addData("Image Data", payload.name());

	auto& images = dynamic_cast<Images&>(_core->requestData(datasetName));
	
	images.setRoi(QRect(QPoint(), payload.size()));
	
	auto& points = _core->requestData<Points&>(images.pointsName());
	
	images.setPoints(&points);

	switch (payload.type()) {
		case ImageCollectionType::Sequence:
		{
			images.setSequence(payload.images(), payload.size());
			break;
		}

		case ImageCollectionType::MultiPartSequence:
		case ImageCollectionType::Stack:
		{
			images.setStack(payload.images(), payload.size());
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