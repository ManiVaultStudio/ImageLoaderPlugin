#include "ImageStackLoader.h"
#include "ImageUtilities.h"

#include <QDebug>
#include <QDir>

ImageStackLoader::ImageStackLoader() :
	ImageCollectionLoader(ImageCollectionType::Stack)
{
}

QStringList ImageStackLoader::dimensionNames() const
{
	auto dimensionNames = QStringList();
	/*
	foreach(const QString& imageFilePath, _imageFilePaths) {
		dimensionNames << QFileInfo(imageFilePath).fileName();
	}
	*/
	return dimensionNames;
}

int ImageStackLoader::noDimensions() const
{
	return noImages();
}

void ImageStackLoader::add(const QString & imageFilePath)
{
//	_imageFilePaths.append(imageFilePath);
}
