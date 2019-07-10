#include "ImageLoader.h"
#include <FreeImage.h>

#include "PointsPlugin.h"
#include "Set.h"

#include <QtCore>
#include <QtDebug>

#include <QFileDialog>
#include <vector>
#include <QInputDialog>

	// #define  FREEIMAGE_LIB

#define FreeImageNamespace 

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
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, "Load Image", "", "Image Files (*.csv *)");
    
    // Don't try to load a file if the dialog was cancelled or the file name is empty
    if (fileName.isNull() || fileName.isEmpty())
        return;

    qDebug() << "Loading image file: " << fileName;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw DataLoadException(fileName, "File was not found at location.");
	
	FreeImageNamespace::FREE_IMAGE_FORMAT formato = FreeImageNamespace::FreeImage_GetFileType(fileName.toUtf8(), 0);//Automatocally detects the format(from over 20 formats!)
	
	/*
	FIBITMAP* imagen = FreeImage_Load(formato, "image.png");
	
	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

    
	std::vector<float> data;
    int numDimensions = 1;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList tokens = line.split(",");

        numDimensions = tokens.size();
        for (int i = 0; i < tokens.size(); i++)
        {
            if (tokens[i].isEmpty())
                continue;
            float f = tokens[i].toFloat();

            data.push_back(f);
        }
    }

    bool ok;
    QString dataSetName = QInputDialog::getText(nullptr, "Add New Dataset",
        "Dataset name:", QLineEdit::Normal, "DataSet", &ok);

    if (ok && !dataSetName.isEmpty()) {
        QString name = _core->addData("Points", dataSetName);
        const IndexSet& set = dynamic_cast<const IndexSet&>(_core->requestSet(name));
        PointsPlugin& points = set.getData();
        points.data.resize(data.size());
        for (int i = 0; i < points.data.size(); i++) {
            points.data[i] = data[i];
        }
        points.numDimensions = numDimensions;
        qDebug() << "Number of dimensions: " << points.numDimensions;

        _core->notifyDataAdded(name);

        qDebug() << "CSV file loaded. Num data points: " << points.data.size();
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

