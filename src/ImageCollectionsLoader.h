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
	template<typename PointIndexMapper>
	void loadBitmap(FIBITMAP* bitmap, const QSize & imageSize, const PointIndexMapper& pointIndexMapper, FloatVector & pointsData);

	template<typename PointIndexMapper>
	void loadImage(const QString& imageFilePath, const QSize& imageSize, const PointIndexMapper& pointIndexMapper, FloatVector& pointsData);

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