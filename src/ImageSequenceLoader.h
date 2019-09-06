#pragma once

#include "ImageCollectionLoader.h"

#include <QSize>

class ImageSequenceLoader : public ImageCollectionLoader
{
	Q_OBJECT

public:
	ImageSequenceLoader();

	QString	directory() const;
	QString	imageType() const;
	QSize imageSize() const;
	QStringList	dimensionNames() const;
	int noDimensions() const;

	void setDirectory(const QString &directory);
	void setImageType(const QString &imageType);
	void setImageSize(const QSize &imageSize);

public:
	void scan();
	void load();

protected:
	void scanDir(const QString &directory);
	void addFile(const QString &imageFilePath);
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
	QString				_directory;
	QString				_imageType;
	QSize				_imageSize;
};

QDebug operator<<(QDebug dbg, const class ImageSequenceLoader &sequence);