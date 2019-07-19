#include "ImageSequence.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageSequence::ImageSequence() :
	_directory(""),
	_imageType("jpg"),
	_imageSize(28, 28),
	_imageFilePaths()
{
}

ImageSequence::ImageSequence(const ImageSequence &other) :
	_directory(other._directory),
	_imageType(other._imageType),
	_imageSize(other._imageSize),
	_imageFilePaths(other._imageFilePaths)
{
}

ImageSequence::~ImageSequence()
{
}

ImageSequence::ImageSequence(const QString &directory, const QSize &imageSize, const QString &imageType) :
	_directory(directory),
	_imageType(imageType),
	_imageSize(imageSize),
	_imageFilePaths()
{
}

QString ImageSequence::directory() const
{
	return _directory;
}

QString ImageSequence::imageType() const
{
	return _imageType;

}
QSize ImageSequence::imageSize() const
{
	return _imageSize;
}

QStringList ImageSequence::imageFilePaths() const
{
	return _imageFilePaths;
}

void ImageSequence::setDirectory(const QString & directory)
{
	_directory = directory;

	emit directoryChanged(_directory);
	emit becameDirty();
}

void ImageSequence::setImageType(const QString & imageType)
{
	_imageType = imageType;

	emit imageTypeChanged(_imageType);
	emit becameDirty();
}

void ImageSequence::setImageSize(const QSize & imageSize)
{
	_imageSize = imageSize;

	emit imageSizeChanged(_imageSize);
	emit becameDirty();
}

void ImageSequence::addFile(const QString &imageFilePath) {
	_imageFilePaths.append(imageFilePath);

	emit foundImageFile(imageFilePath);
}

void ImageSequence::scan() {

	start();
}

void ImageSequence::scanDir(const QString &directory)
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
	imageFiles.setNameFilters(QStringList() << "*." + _imageType);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		if (imageReader.size() == _imageSize) {
			// qDebug() << "Found image: " << fileList.at(i);

			addFile(path);
			scanDir(path);
		}
	}
}

void ImageSequence::run()
{
	if (_directory.isEmpty() || _imageType.isEmpty() || _imageSize.isEmpty()) {
		return;
	}

	qDebug() << "Scanning " << _directory;

	emit beginScan();

	_imageFilePaths.clear();

	scanDir(_directory);

	emit endScan();
}

QDebug operator<<(QDebug dbg, const ImageSequence &sequence)
{
	dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}