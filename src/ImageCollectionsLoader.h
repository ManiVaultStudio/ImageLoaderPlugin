#pragma once

#include "ImageCollections.h"
#include "SubsampleImageSettings.h"

#include <QObject>
#include <QString>

class ImageCollectionsLoader : public QObject {
	Q_OBJECT

public:
	ImageCollectionsLoader(const ImageCollectionType& type);

	ImageCollectionType type() const;
	SubsampleImageSettings& subsampleImageSettings();

	void load(ImageCollections& imageCollections);

public:
	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant()) const;
	void setSetting(const QString& name, const QVariant& value);

private:
	void loadImage(const QString& imageFilePath, const int& imageIndex, FloatVector& pointsData);

signals:
	void beginLoad();
	void endLoad(ImageCollections& imageCollections);
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);
	void message(const QString& message);

protected:
	QSettings				_settings;

private:
	ImageCollectionType		_type;
	SubsampleImageSettings	_subsampleImageSettings;
};