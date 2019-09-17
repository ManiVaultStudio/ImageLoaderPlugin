#pragma once

#include "ImageCollections.h"
#include "ImageDataSet.h"
#include "SubsampleImageSettings.h"

#include <QObject>
#include <QString>

struct FIBITMAP;

using IndexerFunction = std::function<int(int, int)>;

class ImageCollectionsLoader : public QObject {
	Q_OBJECT

public:
	ImageCollectionsLoader(const ImageCollectionType& type);

	ImageCollectionType type() const;
	SubsampleImageSettings& subsampleImageSettings();

	void load(const ImageCollections& scannedImageCollections);

public:
	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant()) const;
	void setSetting(const QString& name, const QVariant& value);
	QString datasetName() const;
	void setDatasetName(const QString& datasetName);

private:
	void loadBitmap(FIBITMAP* bitmap, const QSize & imageSize, const IndexerFunction& indexer, FloatVector & pointsData);
	void loadImage(const QString& imageFilePath, const QSize& imageSize, const IndexerFunction& indexerFunction, FloatVector& pointsData);
	void loadMultiPartImage(const QString& imageFilePath, const QSize& imageSize, const int& pointIndexOffset, const int& noPointsPerDimension, FloatVector& pointsData);

signals:
	void datasetNameChanged(const QString&);
	void beginLoad();
	void endLoad(ImageDataSet& imageDataSet);
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);
	void message(const QString& message);

protected:
	QSettings				_settings;

private:
	ImageCollectionType		_type;
	QString					_datasetName;
	SubsampleImageSettings	_subsampleImageSettings;
};