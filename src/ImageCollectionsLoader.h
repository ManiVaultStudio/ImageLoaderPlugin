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
	QStringList	imageFilePaths() const;
	int noImages() const;
	SubsampleImageSettings& subsampleImageSettings();
	void reset();

	void load(const ImageCollections& imageCollections);

public:
	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant()) const;
	void setSetting(const QString& name, const QVariant& value);

protected:
	//virtual void loadImage(const QString& imageFilePath, const int& imageIndex, FloatVector& pointsData) = 0;

signals:
	void beginLoad();
	void endLoad(FloatVector& pointsData);
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);
	void message(const QString& message);

protected:
	QSettings				_settings;
	QStringList				_imageFilePaths;

private:
	ImageCollectionType		_type;
	SubsampleImageSettings	_subsampleImageSettings;
};