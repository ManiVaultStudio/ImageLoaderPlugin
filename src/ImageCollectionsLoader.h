#pragma once

#include "ImageCollections.h"
#include "Images.h"
#include "SubsampleImageSettings.h"

#include <QObject>
#include <QString>

namespace fi {
	#include <FreeImage.h>
}

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
	void loadBitmap(fi::FIBITMAP* bitmap, const QSize& imageSize, Image& image);
	void loadImage(const QString &imageFilePath, Images& images);

signals:
	void datasetNameChanged(const QString&);
	void beginLoad();
	void endLoad(Images& images);
	void imageLoaded(const QString &imageFilePath, const int& done, const int& total);
	void message(const QString& message);

protected:
	QSettings				_settings;

private:
	ImageCollectionType		_type;
	QString					_datasetName;
	SubsampleImageSettings	_subsampleImageSettings;
};