#include "ImageLoaderPlugin.h"

#include "ImageSequence.h"

#include "PointsPlugin.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <QImageReader>
#include <vector>
#include <QInputDialog>

#include "ImageLoaderDialog.h"

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoaderPlugin")

ImageLoaderPlugin::~ImageLoaderPlugin(void)
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

void ImageLoaderPlugin::addSequence(const ImageCollectionType& imageCollectionType, const QString &name, const QSize& size, const int& noImages, const int &noDimensions, std::vector<float> &pointsData)
{
	qDebug() << "Adding sequence " << name;

	const auto datasetName = _core->addData("Points", name);

	const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));

	PointsPlugin& points = set.getData();

	points.numDimensions = noDimensions;
	points.data.swap(pointsData);
	
	switch (imageCollectionType) {
		case ImageCollectionType::Sequence:
			points.setMetaProperty("type", "SEQUENCE");
			break;
		case ImageCollectionType::Stack:
			points.setMetaProperty("type", "STACK");
			break;

		default:
			break;
	}
	
	points.setMetaProperty("noImages", noImages);
	points.setMetaProperty("imageSize", size);

	_core->notifyDataAdded(datasetName);
}

LoaderPlugin* ImageLoaderPluginFactory::produce()
{
    return new ImageLoaderPlugin();
}