#include "ImageLoader.h"

#include <FreeImagePlus.h>

#include "PointsPlugin.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <QFileDialog>
#include <vector>
#include <QInputDialog>

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
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, "Load Image", "", "Image Files (*.bmp *.png *.jpg *jpeg)");
    
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

			const auto noPixels = imageWidth * imageHeight;

			QFileInfo fileInfo(fileName);

			QString dataSetName(fileInfo.fileName());

			if (!dataSetName.isEmpty()) {
				QString name = _core->addData("Points", dataSetName);

				const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));

				PointsPlugin& points = set.getData();

				std::vector<float> data;

				const auto noDataElements = noPixels * noColorChannels;

				data.resize(noDataElements);
				points.data.resize(noDataElements);

				unsigned x, y;

				for (y = 0; y < imageHeight; y++) {
					FIRGBAF* bits = (FIRGBAF*)image.getScanLine(y);
					
					for (x = 0; x < imageHeight; x++) {
						const auto pixelIndex = y * imageWidth + x;

						switch (noColorChannels)
						{
							case 1: {
								data[pixelIndex] = bits[y].red;
								break;
							}
							case 3: {
								data[pixelIndex * 3 + 0] = bits[y].red;
								data[pixelIndex * 3 + 1] = bits[y].green;
								data[pixelIndex * 3 + 2] = bits[y].blue;
								break;
							}
							case 4: {
								data[pixelIndex * 4 + 0] = bits[y].red;
								data[pixelIndex * 4 + 1] = bits[y].green;
								data[pixelIndex * 4 + 2] = bits[y].blue;
								data[pixelIndex * 4 + 3] = bits[y].alpha;
								break;
							}
							default:
								break;
						}
					}
				}

				for (int i = 0; i < points.data.size(); i++) {
					points.data[i] = data[i];
				}

				points.numDimensions = noColorChannels;

				_core->notifyDataAdded(name);
			}
		}

		qDebug() << fileName << " loaded";
	}
}

// =============================================================================
// Factory
// =============================================================================

LoaderPlugin* ImageLoaderFactory::produce()
{
    return new ImageLoader();
}

