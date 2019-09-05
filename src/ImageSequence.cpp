#include "ImageSequence.h"
#include "ImageUtilities.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImage.h>

ImageSequence::ImageSequence() :
	ImageCollection(ImageCollection::Type::Sequence),
	_runMode(RunMode::Scan),
	_directory(""),
	_imageType("jpg"),
	_imageSize(28, 28),
	_imageFilePaths()
{
}

ImageSequence::ImageSequence(const ImageSequence &other) :
	ImageCollection(ImageCollection::Type::Sequence),
	_runMode(RunMode::Scan),
	_directory(other._directory),
	_imageType(other._imageType),
	_imageSize(other._imageSize),
	_imageFilePaths(other._imageFilePaths)
{
}

ImageSequence::RunMode ImageSequence::runMode() const
{
	return _runMode;
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

QStringList ImageSequence::dimensionNames() const
{
	auto dimensionNames = QStringList();

	for (int i = 1; i <= noDimensions(); i++) {
		dimensionNames << QString("dim_%1").arg(i);
	}

	return dimensionNames;
}

std::vector<float>& ImageSequence::pointsData()
{
	return _pointsData;
}

int ImageSequence::noDimensions() const
{
	return _imageSize.width() * _imageSize.height();
}

void ImageSequence::setRunMode(const RunMode & runMode)
{
	_runMode = runMode;
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

void ImageSequence::scan()
{
}

void ImageSequence::load()
{
}

void ImageSequence::addFile(const QString &imageFilePath)
{
	_imageFilePaths.append(imageFilePath);

	emit message(QString("Found %1").arg(QFileInfo(imageFilePath).fileName()));
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
			addFile(path);
			scanDir(path);
		}
	}
}

/*
void ImageSequence::run()
{
	switch (_runMode)
	{
		case RunMode::Scan: {
			if (_directory.isEmpty() || _imageType.isEmpty() || _imageSize.isEmpty()) {
				return;
			}

			qDebug() << "Scanning for images in " << _directory;

			emit beginScan();

			_imageFilePaths.clear();

			scanDir(_directory);

			emit endScan();

			break;
		}
		
		case RunMode::Load: {
			qDebug() << "Loading images ";

			emit beginLoad();

			_pointsData.clear();

			_pointsData.reserve(noImages() * noDimensions());

			const auto total = _imageFilePaths.size();

			foreach(const QString &imageFilePath, _imageFilePaths) {
				loadImage(imageFilePath);

				const auto done			= _imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / static_cast<float>(total));
				
				emit message(QString("Loading %1 (%2/%3, %4%)").arg(QFileInfo(imageFilePath).fileName(), QString::number(done), QString::number(total), QString::number(percentage, 'f', 1)));
			}

			// qDebug() << _pointsData;

			emit message(QString("%1 image(s) loaded").arg(total));

			emit endLoad();

			break;
		}

		default:
			break;
	}
}
*/

void ImageSequence::loadImage(const QString & imageFilePath)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);
	
	auto *image = FreeImage_ConvertToGreyscale(freeImageLoad(imageFilePath));
	
	const auto image_type = FreeImage_GetImageType(image);

	// qDebug() << image_type;

	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (unsigned y = 0; y <  FreeImage_GetHeight(image); y++) {
					const BYTE *const bits = FreeImage_GetScanLine(image, y);
					for (unsigned x = 0; x <  FreeImage_GetWidth(image); x++) {
						_pointsData.push_back(static_cast<float>(bits[x]));
					}
				}
			}
			break;
	}
}

void ImageSequence::loadImage(const QString & imageFilePath, const int & imageIndex, std::vector<float>& pointsData)
{
}

QDebug operator<<(QDebug dbg, const ImageSequence &sequence)
{
	dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}