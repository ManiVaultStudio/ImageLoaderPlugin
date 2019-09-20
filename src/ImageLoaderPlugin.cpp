#include "ImageLoaderPlugin.h"
#include "ImageLoaderDialog.h"

#include "PointsPlugin.h"
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

void ImageLoaderPlugin::addImageDataSet(ImageDataSet& imageDataSet)
{
	qDebug() << imageDataSet;
	
	const auto datasetName = _core->addData("Points", imageDataSet.name());

	const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(datasetName));

	//qDebug() << imageDataSet.pointsData() << imageDataSet.pointsData().size();

	PointsPlugin& points = set.getData();

	points.numDimensions = imageDataSet.noDimensions();
	points.data.swap(imageDataSet.pointsData());
	points.dimNames.clear();

	foreach(const QString& dimensionName, imageDataSet.dimensionNames()) {
		points.dimNames.push_back(dimensionName);
	}

	switch (imageDataSet.type()) {
		case ImageCollectionType::Sequence:
			points.setProperty("type", "SEQUENCE");
			break;
			
		case ImageCollectionType::Stack:
			points.setProperty("type", "STACK");
			break;

		case ImageCollectionType::MultiPartSequence:
			points.setProperty("type", "MULTI_PART_SEQUENCE");
			break;

		default:
			break;
	}

	points.setProperty("imageFilePaths", imageDataSet.imageFilePaths());
	points.setProperty("imageSizes", imageDataSet.imageSizes());

	_core->notifyDataAdded(datasetName);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}