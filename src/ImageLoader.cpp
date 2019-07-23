#include "ImageLoader.h"

#include "ImageSequence.h"

#include "PointsPlugin.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <QImageReader>
#include <vector>
#include <QInputDialog>

#include "SequenceDialog.h"

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoader")

ImageLoader::~ImageLoader(void)
{
}

void ImageLoader::init()
{
}

void ImageLoader::loadData()
{
	SequenceDialog dialog(this);
	
	dialog.exec();
}

void ImageLoader::addSequence(const QString &name, const int &noDimensions, std::vector<float> &pointsData)
{
	qDebug() << "Adding sequence " << name;

	const auto datasetName = _core->addData("Points", name);

	const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));

	PointsPlugin& points = set.getData();

	//points.data.resize(pointsData.size());
	points.data.swap(pointsData);
	//for (int i = 0; i < points.data.size(); i++) {
	//	points.data[i] = pointsData[i];
	//}

	points.numDimensions = noDimensions;

	_core->notifyDataAdded(datasetName);
}

LoaderPlugin* ImageLoaderFactory::produce()
{
    return new ImageLoader();
}