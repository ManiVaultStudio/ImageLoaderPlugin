#pragma once

#include "SubsampleImageSettings.h"

#include <QObject>
#include <QString>

class ImageCollectionLoader : public QObject {
	Q_OBJECT

public:
	ImageCollectionLoader(const ImageCollectionType& type);

	static QString typeName(const ImageCollectionType& type);

	ImageCollectionType type() const;
	QStringList	imageFilePaths() const;
	int noImages() const;
	SubsampleImageSettings& subsampleImageSettings();
	void reset();

//	virtual int noDimensions() const = 0;
//	virtual QStringList	dimensionNames() const = 0;
	virtual void load() = 0;

public:
	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant()) const;
	void setSetting(const QString& name, const QVariant& value);

protected:
	virtual void loadImage(const QString& imageFilePath, const int& imageIndex, FloatVector& pointsData) = 0;

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