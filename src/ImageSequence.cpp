#include "ImageSequence.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

#include <FreeImagePlus.h>

ImageSequence::ImageSequence() :
	_runMode(RunMode::Scan),
	_directory(""),
	_imageType("jpg"),
	_imageSize(28, 28),
	_imageFilePaths()
{
}

ImageSequence::ImageSequence(const ImageSequence &other) :
	_runMode(RunMode::Scan),
	_directory(other._directory),
	_imageType(other._imageType),
	_imageSize(other._imageSize),
	_imageFilePaths(other._imageFilePaths)
{
}

ImageSequence::~ImageSequence()
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

QStringList ImageSequence::imageFilePaths() const
{
	return _imageFilePaths;
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

void ImageSequence::addFile(const QString &imageFilePath) {
	_imageFilePaths.append(imageFilePath);

	emit foundImageFile(imageFilePath);
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

			foreach(const QString &imageFilePath, _imageFilePaths) {
				loadImage(imageFilePath);
				qDebug() << _imageFilePaths.indexOf(imageFilePath);
			}
			// _imageFilePaths.clear();
			// scanDir(_directory);

			emit endLoad();

			break;
		}

		default:
			break;
	}
	
}

void ImageSequence::loadImage(const QString & imageFilePath)
{
	fipImage image;

	// qDebug() << "Loading image file: " << imageFilePath;

	if (image.load(imageFilePath.toUtf8())) {
		/*
		const auto colorType = image.getColorType();

		int noColorChannels = 0;

		switch (colorType)
		{
		case FIC_MINISBLACK:
		case FIC_MINISWHITE:
		case FIC_PALETTE: {
			noColorChannels = 1;
			break;
		}
		case FIC_RGB: {
			noColorChannels = 3;
			break;
		}
		case FIC_RGBALPHA:
		case FIC_CMYK: {
			noColorChannels = 4;
			break;
		}
		default:
			break;
		}

		qDebug() << "Number of color channels: " << noColorChannels;

		if (convertToGrayscale) {
			qDebug() << "Converting image to grayscale";

			image.convertToGrayscale();

			noColorChannels = 1;
		}
		*/

		image.convertToGrayscale();

		// const auto imageWidth	= image.getWidth();
		// const auto imageHeight	= image.getHeight();

		// qDebug() << "Image dimensions: " << imageWidth << " x " << imageHeight;

		// const auto noPixels = _imageSize.width() * _imageSize.height();
		// const auto numDimensions = noPixels;

		// QFileInfo fileInfo(imageFilePath);

		// QString dataSetName(fileInfo.fileName());

		/*
		if (!dataSetName.isEmpty()) {
			QString name = _core->addData("Points", dataSetName);

			const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));

			PointsPlugin& points = set.getData();

			std::vector<float> data;

			const auto noDataElements = noPixels * numDimensions;

			//data.resize(noDataElements);
			points.data.resize(noDataElements);

			unsigned x, y;

			for (y = 0; y < imageHeight; y++) {
				FIRGBAF* bits = (FIRGBAF*)image.getScanLine(y);

				for (x = 0; x < imageHeight; x++) {
					const auto pixelIndex = y * imageWidth + x;

					// data[pixelIndex + 0] = x;
					// data[pixelIndex + 1] = y;
					data.push_back(x);
					data.push_back(y);

					switch (noColorChannels)
					{
					case 3: {
						data.push_back(bits[y].red);
						data.push_back(bits[y].green);
						data.push_back(bits[y].blue);
						// data[pixelIndex * 3 + 2] = bits[y].red;
						// data[pixelIndex * 3 + 3] = bits[y].green;
						// data[pixelIndex * 3 + 4] = bits[y].blue;
						break;
					}
					case 4: {
						data.push_back(bits[y].red);
						data.push_back(bits[y].green);
						data.push_back(bits[y].blue);
						data.push_back(bits[y].alpha);
						// dapush_back(ta[pixelIndex * 4 + 2] = bits[y].red;
						// dapush_back(ta[pixelIndex * 4 + 3] = bits[y].green;
						// data[pixelIndex * 4 + 4] = bits[y].blue;
						// data[pixelIndex * 4 + 5] = bits[y].alpha;
						break;
					}
					default:
						break;
					}
				}
			}

			qDebug() << data;

			for (int i = 0; i < points.data.size(); i++) {
				points.data[i] = data[i];
			}


			std::vector<QString> dimNames;

			switch (noColorChannels)
			{
				case 3: {
					dimNames = { "X", "Y", "Red", "Green", "Blue" };
					break;
				}
				case 4: {
					dimNames = { "X", "Y", "Red", "Green", "Blue", "Aplha" };
					break;
				}
				default:
					break;
			}

			points.dimNames = dimNames;
			points.numDimensions = numDimensions;

			_core->notifyDataAdded(name);

			qDebug() << name << " added with " << points.numDimensions << " dimensions and " << points.getNumPoints() << " points";
		}
		*/
		/*
		bool converted = false;

		if (noColorChannels == 1) {
			image.convertToFloat();
		}
		else
		{
			switch (noColorChannels)
			{
			case 3: {
				converted = image.convertToRGBF();
				break;
			}
			case 4: {
				converted = image.convertToRGBAF();
				break;
			}
			default:
				break;
			}
		}
		*/
		/*
		if (converted) {

		}
		*/
	}
}

QDebug operator<<(QDebug dbg, const ImageSequence &sequence)
{
	dbg << sequence.imageSize() << ", " << sequence.imageType() << ", " << sequence.directory();

	return dbg;
}