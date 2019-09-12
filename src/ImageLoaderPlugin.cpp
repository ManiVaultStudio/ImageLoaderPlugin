#include "ImageLoaderPlugin.h"

#include "PointsPlugin.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <QImageReader>
#include <vector>
#include <QInputDialog>

#include "ImageLoaderDialog.h"

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

void ImageLoaderPlugin::addDataSet(ImageCollections& imageCollections)
{
	qDebug() << QString("Adding data set %1 with %2 dimensions").arg(imageCollections.datasetName(), QString::number(imageCollections.noDimensions()));

	const auto name			= imageCollections.datasetName();
	const auto datasetName	= _core->addData("Points", name);

	const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));

	PointsPlugin& points = set.getData();

	points.numDimensions = imageCollections.noDimensions();
	points.data.swap(imageCollections.pointsData());

	/*
	points.dimNames.clear();

	foreach(const QString &dimensionName, dimensionNames) {
		points.dimNames.push_back(dimensionName);
	}
	*/

	switch (imageCollections.type()) {
		case ImageCollectionType::Sequence:
		{
			const auto sequence = imageCollections.map().first();

			points.setProperty("type", "SEQUENCE");
			points.setProperty("noImages", sequence.noImages());
			points.setProperty("imageSize", sequence.imageSize());
			break;
		}
			
		case ImageCollectionType::Stack:
		{
			const auto stack = imageCollections.map().first();

			points.setProperty("type", "STACK");
			points.setProperty("noImages", stack.noImages());
			points.setProperty("imageSize", stack.imageSize());
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