#include "ImageStackScan.h"

#include <QDebug>
#include <QDir>

ImageStackScan::ImageStackScan(QSettings* settings) :
	ImageScan(settings)
{
	/*
	const auto directory = _imageStack.setting("Directory", "").toString();

	if (QDir(directory).exists()) {
		_imageStackScan.setDirectory(directory);
	}
	*/
}

void ImageStackScan::scan()
{
	// QMap<QString, QStringList>				_stacks;
}