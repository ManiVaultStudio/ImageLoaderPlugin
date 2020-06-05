#include "ImageSequenceScanner.h"
#include "ImageLoaderPlugin.h"

#include <QDebug>
#include <QDir>
#include <QImageReader>

ImageSequenceScanner::ImageSequenceScanner() :
	ImageScanner(ImageData::Type::Sequence),
	_square(true)
{
	auto supportedImageTypes = QStringList();

	supportedImageTypes << "jpg" << "png" << "bmp" << "tif" << "tiff";

	setSupportedImageTypes(supportedImageTypes);
}

void ImageSequenceScanner::loadSettings()
{
	ImageScanner::loadSettings();
}

void ImageSequenceScanner::scan()
{
	std::vector<ImageCollection> sequences;

	QStringList nameFilters;

	for (const auto& supportedImageType : supportedImageTypes())
		nameFilters << "*." + supportedImageType;

	qDebug() << nameFilters;

	scanDir(_directory, nameFilters, sequences);

	auto& imageCollectionsModel = _imageLoaderPlugin->imageCollectionsModel();
	
	imageCollectionsModel.clear();
	imageCollectionsModel.insert(0, sequences);
}

auto ImageSequenceScanner::findSequence(std::vector<ImageCollection>& sequences, const QString& imageType, const QSize& imageSize)
{
	return std::find_if(sequences.begin(), sequences.end(), [&imageType, &imageSize](const auto& sequence) {
		return sequence.imageType(Qt::EditRole).toString() == imageType && sequence.sourceSize(Qt::EditRole).toSize() == imageSize;
	});
}

void ImageSequenceScanner::scanDir(const QString& directory, QStringList nameFilters, std::vector<ImageCollection>& sequences)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		scanDir(path, nameFilters, sequences);
	}
	
	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto fileName			= fileList.at(i);
		const auto imageFilePath	= QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileName);
		const auto imageType		= QFileInfo(fileName).suffix();

		QImageReader imageReader(imageFilePath);

		const auto imageSize = imageReader.size();
		
		auto it = findSequence(sequences, imageType, imageSize);

		if (it == sequences.end()) {
			auto imageCollection = ImageCollection(_directory, imageType, imageSize);

			imageCollection.addImage(imageFilePath);

			sequences.push_back(imageCollection);

			qDebug() << "Not found";
		}
		else {
			
			(*it).addImage(imageFilePath);
		}
	}
}