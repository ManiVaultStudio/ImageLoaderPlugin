#include "ImageStack.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImage.h>

ImageStack::ImageStack() :
	_runMode(RunMode::Scan),
	_directory(""),
	_imageType("jpg"),
	_imageSize(28, 28),
	_imageFilePaths()
{
}

ImageStack::ImageStack(const ImageStack &other) :
	_runMode(RunMode::Scan),
	_directory(other._directory),
	_imageType(other._imageType),
	_imageSize(other._imageSize),
	_imageFilePaths(other._imageFilePaths)
{
}

ImageStack::~ImageStack()
{
}

ImageStack::RunMode ImageStack::runMode() const
{
	return _runMode;
}

QString ImageStack::directory() const
{
	return _directory;
}

QString ImageStack::imageType() const
{
	return _imageType;

}
QSize ImageStack::imageSize() const
{
	return _imageSize;
}

QStringList ImageStack::imageFilePaths() const
{
	return _imageFilePaths;
}

std::vector<float>& ImageStack::pointsData()
{
	return _pointsData;
}

int ImageStack::noDimenions() const
{
	return _imageSize.width() * _imageSize.height();
}

void ImageStack::setRunMode(const RunMode & runMode)
{
	_runMode = runMode;
}

void ImageStack::setDirectory(const QString & directory)
{
	_directory = directory;

	emit directoryChanged(_directory);
	emit becameDirty();
}

void ImageStack::setImageType(const QString & imageType)
{
	_imageType = imageType;

	emit imageTypeChanged(_imageType);
	emit becameDirty();
}

void ImageStack::setImageSize(const QSize & imageSize)
{
	_imageSize = imageSize;

	emit imageSizeChanged(_imageSize);
	emit becameDirty();
}

void ImageStack::addFile(const QString &imageFilePath)
{
	_imageFilePaths.append(imageFilePath);

	emit message(QString("Found %1").arg(QFileInfo(imageFilePath).fileName()));
}

void ImageStack::scanDir(const QString &directory)
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

void ImageStack::run()
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

			const auto total = _imageFilePaths.size();

			foreach(const QString &imageFilePath, _imageFilePaths) {
				loadImage(imageFilePath);

				const auto done			= _imageFilePaths.indexOf(imageFilePath) + 1;
				const auto percentage	= 100.0f * (done / (float)total);
				
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

void ImageStack::loadImage(const QString & imageFilePath)
{
	const auto format = FreeImage_GetFileType(imageFilePath.toUtf8(), 0);
	
	auto *image = FreeImage_ConvertToGreyscale(FreeImage_Load(format, imageFilePath.toUtf8()));
	
	unsigned x, y;

	const auto image_type = FreeImage_GetImageType(image);

	// qDebug() << image_type;

	switch (image_type) {
		case FIT_BITMAP:
			if (FreeImage_GetBPP(image) == 8) {
				for (y = 0; y <  FreeImage_GetHeight(image); y++) {
					BYTE *bits = (BYTE *)FreeImage_GetScanLine(image, y);
					for (x = 0; x <  FreeImage_GetWidth(image); x++) {
						_pointsData.push_back((float)bits[x]);
					}
				}
			}
			break;

		case FIT_UINT16:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				unsigned short *bits = (unsigned short *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_INT16:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				short *bits = (short *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_UINT32:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				DWORD *bits = (DWORD *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_INT32:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				LONG *bits = (LONG *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x]);
				}
			}
			break;
			
		case FIT_FLOAT:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				float *bits = (float *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x]);
				}
			}
			break;

		case FIT_DOUBLE:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				double *bits = (double*)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x]);
				}
			}
			break;
			
		case FIT_RGB16:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				FIRGB16 *bits = (FIRGB16 *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x].red);
				}
			}
			break;

		case FIT_RGBF:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				FIRGBF *bits = (FIRGBF *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x].red);
				}
			}
			break;

		case FIT_RGBA16:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				FIRGBA16 *bits = (FIRGBA16 *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x].red);
				}
			}
			break;

		case FIT_RGBAF:
			for (y = 0; y <  FreeImage_GetHeight(image); y++) {
				FIRGBAF *bits = (FIRGBAF *)FreeImage_GetScanLine(image, y);
				for (x = 0; x <  FreeImage_GetWidth(image); x++) {
					_pointsData.push_back((float)bits[x].red);
				}
			}
			break;
	}
}

QDebug operator<<(QDebug dbg, const ImageStack &sequence)
{
	dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}