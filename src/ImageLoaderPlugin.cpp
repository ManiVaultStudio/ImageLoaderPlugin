#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"
#include "Scanned.h"

#include "ImageData/Images.h"
#include "Set.h"

#include <QtCore>
#include <QDebug>

#include "PointData.h"

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoaderPlugin")

ImageLoaderPlugin::ImageLoaderPlugin() :
	LoaderPlugin("Image Loader"),
	_imageCollectionsModel()
{
	qRegisterMetaType<std::shared_ptr<QImage>>("std::shared_ptr<QImage>");
	qRegisterMetaType<std::shared_ptr<Payload>>("std::shared_ptr<Payload>");

	/*
	ImageCollection imageCollection("C:\test", "jpg", QSize(32, 32));

	imageCollection.setDatasetName("Thomas");

	imageCollection.addImage("C:\test\a.jpg");
	imageCollection.addImage("C:\test\b.jpg");

	_imageCollectionsModel.insert(0, std::vector<ImageCollection>({ imageCollection }));
	*/
}

void ImageLoaderPlugin::init()
{
}

void ImageLoaderPlugin::loadData()
{
	ImageLoaderDialog dialog;

	dialog.initialize(this);
	
	dialog.exec();
}

QString ImageLoaderPlugin::addImages(std::shared_ptr<Payload> payload)
{
	const auto datasetName = _core->addData("Images", payload->name());

	auto& images = dynamic_cast<Images&>(_core->requestData(datasetName));
	
	auto& points = _core->requestData<Points&>(images.pointsName());
	
	images.setPoints(&points);

	switch (payload->type()) {
		case ImageData::Type::Sequence:
		{
			images.setSequence(payload->images(), payload->size());
			break;
		}

		case ImageData::Type::MultiPartSequence:
		case ImageData::Type::Stack:
		{
			images.setStack(payload->images(), payload->size());
			break;
		}
			
		default:
			break;
	}
	
	_core->notifyDataAdded(datasetName);

	return datasetName;
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}