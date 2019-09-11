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
	qDebug() << QString("Adding data set %1 with %2 dimensions").arg(imageCollections.name(), QString::number(imageCollections.noDimensions()));

	const auto name			= imageCollections.name();
	const auto datasetName	= _core->addData("Points", name);

	const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));

	PointsPlugin& points = set.getData();

	points.numDimensions = imageCollections.noDimensions();
	points.data.swap(imageCollections.pointsData());

	/*
	for (int i = 0; i <= 200; i++) {
		qDebug() << imageCollections.pointsData()[i];
	}
	*/

	/*
	points.dimNames.clear();

	foreach(const QString &dimensionName, dimensionNames) {
		points.dimNames.push_back(dimensionName);
	}

	switch (imageCollectionType) {
		case ImageCollectionType::Sequence:
			points.setProperty("type", "SEQUENCE");
			break;
		case ImageCollectionType::Stack:
			points.setProperty("type", "STACK");
			break;

		default:
			break;
	}
	points.setProperty("noImages", noImages);
	points.setProperty("imageSize", size);
	*/

	_core->notifyDataAdded(datasetName);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}