#include "ImageSequence.h"

#include <QDebug>

ImageSequence::ImageSequence() :
	_directory(""),
	_imageType("jpg"),
	_imageSize(28, 28),
	_imageFilePaths(),
	_dirty(true)
{
}

ImageSequence::ImageSequence(const ImageSequence &other) :
	_directory(other._directory),
	_imageType(other._imageType),
	_imageSize(other._imageSize),
	_imageFilePaths(other._imageFilePaths),
	_dirty(true)
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

	emit directoryChanged(directory);

	_dirty = true;
}

void ImageSequence::setImageType(const QString & imageType)
{
	_imageType = imageType;
	_dirty = true;
}

void ImageSequence::setImageSize(const QSize & imageSize)
{
	_imageSize = imageSize;
	_dirty = true;
}

void ImageSequence::addFile(const QString &imageFilePath) {
	_imageFilePaths.append(imageFilePath);
}

void ImageSequence::scan() {
	if (_directory.isEmpty() || _imageType.isEmpty() || _imageSize.isEmpty()) {
		return;
	}

	qDebug() << "Scanning " << _directory;


}

QDebug operator<<(QDebug dbg, const ImageSequence &sequence)
{
	dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}