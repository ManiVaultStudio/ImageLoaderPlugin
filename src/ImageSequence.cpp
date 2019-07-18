#include "ImageSequence.h"

#include <QDebug>

ImageSequence::ImageSequence()
{
}

ImageSequence::ImageSequence(const ImageSequence &other) :
	_size(other._size),
	_type(other._type),
	_directory(other._directory) {
}

ImageSequence::~ImageSequence()
{
}

ImageSequence::ImageSequence(const QSize &size, const QString &type, const QString &directory) :
	_size(size),
	_type(type),
	_directory(directory) {
}

QDebug operator<<(QDebug dbg, const ImageSequence &sequence)
{
	dbg << sequence.size() << ", " << sequence.type() << ", " << sequence.directory();

	return dbg;
}

QSize ImageSequence::size() const
{
	return _size;
}

QString ImageSequence::type() const
{
	return _type;
}

QString ImageSequence::directory() const
{
	return _directory;
}

QStringList ImageSequence::files() const
{
	return _files;
}

void ImageSequence::addFile(const QString &imageFilePath) {
	_files.append(imageFilePath);
}