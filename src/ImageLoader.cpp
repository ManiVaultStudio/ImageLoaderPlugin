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

void ImageLoader::addSequence(const QString &name, const QStringList &imageFilePaths)
{
	qDebug() << "Adding sequence " << name;

	const auto datasetName = _core->addData("Points", name);

	
	/**/
}

LoaderPlugin* ImageLoaderFactory::produce()
{
    return new ImageLoader();
}