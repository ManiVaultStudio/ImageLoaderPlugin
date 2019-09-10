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

void ImageLoaderPlugin::addSequence(const ImageCollectionType& imageCollectionType, const QString &name, const QSize& size, const int& noImages, const int &noDimensions, std::vector<float> &pointsData, const QStringList& dimensionNames /*= QStringList()*/)
{
	qDebug() << "Adding sequence " << name;

	const auto datasetName = _core->addData("Points", name);

	const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));

	PointsPlugin& points = set.getData();

	points.numDimensions = noDimensions;
	points.data.swap(pointsData);

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

	_core->notifyDataAdded(datasetName);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}