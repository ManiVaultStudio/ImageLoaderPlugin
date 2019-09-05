#pragma once

#include "ResampleImageSettings.h"

#include <QThread>
#include <QString>

class ImageCollection : public QThread {
	Q_OBJECT

public:
	enum class Type
	{
		Sequence,
		Stack,
		MultiPartSequence
	};

	ImageCollection(const Type& type);

	virtual Type type() const = 0;
	virtual QStringList	imageFilePaths() const = 0;
	virtual int noDimensions() const = 0;
	virtual QStringList	dimensionNames() const = 0;
	virtual int noImages() const = 0;
	virtual void load() = 0;
	
	static QString typeName(const Type& type);

protected:
	virtual void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData) = 0;

signals:
	void beginLoad(ImageCollection* imageCollection);
	void endLoad(ImageCollection* imageCollection, std::vector<float>& pointsData);
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);
	void message(const QString& message);

private:
	QSettings				_settings;
	Type					_type;
	QStringList				_imageFilePaths;
	ResampleImageSettings	_resampleImageSettings;
};