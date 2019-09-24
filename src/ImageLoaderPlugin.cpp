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

void ImageLoaderPlugin::addImagePointDataSet(ImagePointDataSet& imagePointDataSet)
{
	qDebug() << imagePointDataSet;
	
	const auto datasetName = _core->addData("Points", imagePointDataSet.name());

	const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(datasetName));

	PointsPlugin& points = set.getData();

	auto noPoints = 0;

	switch (imagePointDataSet.type()) {
		case ImageCollectionType::Sequence:
			points.setProperty("type", "SEQUENCE");
			noPoints = imagePointDataSet.noImages();
			break;

		case ImageCollectionType::Stack:
			points.setProperty("type", "STACK");
			noPoints = imagePointDataSet.noPointsPerDimension();
			break;

		case ImageCollectionType::MultiPartSequence:
			points.setProperty("type", "MULTI_PART_SEQUENCE");
			noPoints = imagePointDataSet.noPointsPerDimension();
			break;

		default:
			break;
	}

	points.setData(imagePointDataSet.pointsData().data(), noPoints, imagePointDataSet.noDimensions());
	points.setDimensionNames(imagePointDataSet.dimensionNames().toVector().toStdVector());
	points.setProperty("imageFilePaths", imagePointDataSet.imageFilePaths());
	points.setProperty("imageSizes", imagePointDataSet.imageSizes());

	_core->notifyDataAdded(datasetName);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}