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

	Type type() const;
	QStringList	imageFilePaths() const;
	int noImages() const;
	ResampleImageSettings& resampleImageSettings();
	void reset();

	virtual int noDimensions() const = 0;
	virtual QStringList	dimensionNames() const = 0;
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
	QStringList				_imageFilePaths;

private:
	QSettings				_settings;
	Type					_type;
	ResampleImageSettings	_resampleImageSettings;
};