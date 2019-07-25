#include "ImageStack.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImage.h>

QString Layer::imageName() const {
	return "Temp";
}

ImageStackModel::ImageStackModel() :
	QAbstractListModel()
{
	//_layers = layers;
}

int ImageStackModel::rowCount(const QModelIndex& parent) const {
	return _layers.size();
}

QVariant ImageStackModel::data(const QModelIndex& index, int role) const {
	// Check that the index is valid and within the correct range first:
	if (!index.isValid()) return QVariant();
	
	//if (index.row() >= decks_.size()) return QVariant();

	if (role == Qt::DisplayRole) {
		// Only returns something for the roles you support (DisplayRole is a minimum)
		// Here we assume that the "Employee" class has a "lastName" method but of course any string can be returned
		return QVariant(_layers.at(index.row())->imageName());
	}
	else {
		return QVariant();
	}
}

ImageStack::ImageStack() :
	_runMode(RunMode::Scan),
	_directory(""),
	_imageTypes(),
	_imageFilePaths()
{
}

ImageStack::ImageStack(const ImageStack &other) :
	_runMode(RunMode::Scan),
	_directory(other._directory),
	_imageTypes(other._imageTypes),
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

QStringList ImageStack::imageTypes() const
{
	return _imageTypes;

}

QStringList ImageStack::imageFilePaths() const
{
	return _imageFilePaths;
}

QMap<QString, QStringList> ImageStack::stacks() const
{
	return _stacks;
}

std::vector<float>& ImageStack::pointsData()
{
	return _pointsData;
}

ImageStackModel& ImageStack::model()
{
	return _model;
}

int ImageStack::noDimenions() const
{
	return 100;// _imageSize.width() * _imageSize.height();
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

void ImageStack::setImageTypes(const QStringList & imageTypes)
{
	_imageTypes = imageTypes;

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
			const auto sizeString = QString("(%1,%2)").arg(QString::number(imageReader.size().width()), QString::number(imageReader.size().height()));

			if (_stacks.contains(sizeString)) {
				_stacks[sizeString] = QStringList();
			}

			_stacks[sizeString] << path;
		}
		
		addFile(path);
		scanDir(path);
	}
}

void ImageStack::run()
{
	switch (_runMode)
	{
		case RunMode::Scan: {
			if (_directory.isEmpty() || _imageTypes.isEmpty()) {
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
	dbg << ", " << sequence.imageTypes() << ", " << sequence.directory();

	return dbg;
}