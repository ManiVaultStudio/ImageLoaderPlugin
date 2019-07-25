#include "ImageStacks.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImage.h>

ImageStacks::ImageStacks() :
	_directory(""),
	_imageTypes()
{
}

ImageStacks::ImageStacks(const ImageStacks &other) :
	_directory(other._directory),
	_imageTypes(other._imageTypes)
{
}

ImageStacks::~ImageStacks()
{
}

QString ImageStacks::directory() const
{
	return _directory;
}

QStringList ImageStacks::imageTypes() const
{
	return _imageTypes;

}

ImageStackMap& ImageStacks::stacks()
{
	return _stacks;
}

void ImageStacks::setDirectory(const QString & directory)
{
	_directory = directory;

	emit directoryChanged(_directory);
	emit becameDirty();
}

void ImageStacks::setImageTypes(const QStringList & imageTypes)
{
	_imageTypes = imageTypes;

	emit becameDirty();
}

void ImageStacks::scanDir(const QString &directory)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		scanDir(path);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);

	auto nameFilters = QStringList();

	foreach(QString imageType, _imageTypes)
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		// qDebug() << imageReader.size();

		if (!imageReader.size().isEmpty()) {
			const auto sizeString = QString("%1x%2").arg(QString::number(imageReader.size().width()), QString::number(imageReader.size().height()));

			if (_stacks.contains(sizeString)) {
				_stacks[sizeString] = ImageStack(imageReader.size());
			}

			_stacks[sizeString].add(path);
		}
		
		scanDir(path);
	}
}

void ImageStacks::run()
{
	if (_directory.isEmpty() || _imageTypes.isEmpty()) {
		return;
	}

	qDebug() << "Scanning for images in " << _directory;

	emit beginScan();

	_stacks.clear();

	scanDir(_directory);

	emit endScan();
}

QDebug operator<<(QDebug dbg, const ImageStacks &sequence)
{
	dbg << ", " << sequence.imageTypes() << ", " << sequence.directory();

	return dbg;
}