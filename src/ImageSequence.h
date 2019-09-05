#pragma once

#include "ImageCollection.h"

#include <QThread>
#include <QSize>

class ImageSequence : public ImageCollection
{
	Q_OBJECT

public:
	ImageSequence();
	ImageSequence(const ImageSequence &other);

	enum RunMode
	{
		Scan,
		Load
	};

	RunMode	runMode() const;
	QString	directory() const;
	QString	imageType() const;
	QSize imageSize() const;
	QStringList	dimensionNames() const;
	std::vector<float>& pointsData();
	int noDimensions() const;

	void setRunMode(const RunMode &runMode);
	void setDirectory(const QString &directory);
	void setImageType(const QString &imageType);
	void setImageSize(const QSize &imageSize);

public:
	void scan();
	void load();

protected:
	void scanDir(const QString &directory);
	void addFile(const QString &imageFilePath);
	void loadImage(const QString & imageFilePath);
	void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData);

signals:
	void becameDirty();
	void beginScan();
	void endScan();
	void directoryChanged(const QString &directory);
	void imageTypeChanged(const QString &imageType);
	void imageSizeChanged(const QSize &imageSize);
	void imageFilePathsChanged(const QStringList &imageFilePaths);
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);

private:
	RunMode				_runMode;
	QString				_directory;
	QString				_imageType;
	QSize				_imageSize;
	QStringList			_imageFilePaths;
	std::vector<float>	_pointsData;
};

QDebug operator<<(QDebug dbg, const class ImageSequence &sequence);