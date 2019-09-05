#pragma once

#include "ResampleImageSettings.h"

#include <QObject>
#include <QString>

class ImageCollection : public QObject {
	Q_OBJECT

public:
	enum class Type
	{
		Sequence,
		Stack,
		MultiPartSequence
	};

	ImageCollection(const Type& type);

	static QString typeName(const Type& type);

	virtual Type type() const;
	virtual QStringList	imageFilePaths() const;
	virtual int noImages() const;

	virtual int noDimensions() const = 0;
	virtual QStringList	dimensionNames() const = 0;
	virtual void load() = 0;

protected:
	virtual void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData) = 0;

signals:
	void beginLoad();
	void endLoad();
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);
	void message(const QString& message);

private:
	QSettings				_settings;
	Type					_type;
	QStringList				_imageFilePaths;
	ResampleImageSettings	_resampleImageSettings;
};