#include "ImageLoader.h"

#include "ImageSequence.h"

#include <FreeImagePlus.h>

#include "PointsPlugin.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <QImageReader>
#include <vector>
#include <QInputDialog>

#include "SequenceDialog.h"

Q_PLUGIN_METADATA(IID "nl.tudelft.ImageLoader")

// =============================================================================
// View
// =============================================================================

ImageLoader::~ImageLoader(void)
{
    
}

void ImageLoader::init()
{

}

void ImageLoader::loadData()
{
	SequenceDialog dialog;

	
	dialog.exec();

	/*
	const auto directory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose sequence directory");

	if (directory.isNull() || directory.isEmpty())
		return;

	const auto splitDirectories = false;
	const auto splitImageType	= true;

	QDirIterator it(directory, QStringList() << "*.jpg", QDir::Files, QDirIterator::Subdirectories);
	
	QMap<QString, ImageSequence> sequences;

	while (it.hasNext()) {
		const auto filePath = it.next();

		QFileInfo fileinfo(filePath);

		QImageReader reader(it.next());

		const auto imageSize	= reader.size();
		const auto imageType	= fileinfo.completeSuffix();
		const auto directory	= fileinfo.absoluteDir().absolutePath();
		const auto sequence		= ImageSequence(imageSize, "jpg", directory);

		/*
		auto sequenceName = 
		if (splitDirectories == true) {
			sequenceName.append);
		}

		if (splitImageType == true) {
			sequenceName.append(fileinfo.completeSuffix());
		}

		if (!sequences.contains(sequenceName)) {
			sequences.insert(sequenceName, );
		}

		sequences[sequenceName].addFile(it.next());
		

		// qDebug() << imageSize << sequenceName;
	}

	// qDebug() << sequences;

		



    // Don't try to load a file if the dialog was cancelled or the file name is empty
    if (fileName.isNull() || fileName.isEmpty())
        return;
	
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw DataLoadException(fileName, "File was not found at location.");
	
	const auto convertToGrayscale = false;

	fipImage image;

	qDebug() << "Loading image file: " << fileName;

	if (image.load(fileName.toUtf8())) {
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

		if (converted) {
			const auto imageWidth = image.getWidth();
			const auto imageHeight = image.getHeight();

			qDebug() << "Image dimensions: " << imageWidth << " x " << imageHeight;

			const auto noPixels			= imageWidth * imageHeight;
			const auto numDimensions	= noColorChannels + 2;

			QFileInfo fileInfo(fileName);

			QString dataSetName(fileInfo.fileName());

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
				
				points.dimNames			= dimNames;
				points.numDimensions	= numDimensions;

				_core->notifyDataAdded(name);

				qDebug() << name << " added with " << points.numDimensions << " dimensions and " << points.getNumPoints() << " points";
			}
		}

		qDebug() << fileName << " loaded";
	}
	*/
}

// =============================================================================
// Factory
// =============================================================================

LoaderPlugin* ImageLoaderFactory::produce()
{
    return new ImageLoader();
}

