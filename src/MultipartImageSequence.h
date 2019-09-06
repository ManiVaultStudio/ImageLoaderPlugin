#pragma once

#include "ImageCollectionLoader.h"

#include <QSize>
#include <QMap>

class MultipartImageSequence : public ImageCollectionLoader
{
	Q_OBJECT

public:
	MultipartImageSequence();

	QString	directory() const;
	QStringList	dimensionNames() const;
	std::vector<float>& pointsData();
	int noDimensions() const;
	void setDirectory(const QString &directory);

public:
	void scan();
	void load() override;

protected:
	void scanDir(const QString &directory);
	void addFile(const QString &imageFilePath);
	void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData);
	
signals:
	void becameDirty();
	void directoryChanged(const QString &directory);
	void imageFilePathsChanged(const QStringList &imageFilePaths);

private:
	QString				_directory;
	std::vector<float>	_pointsData;
};

QDebug operator<<(QDebug dbg, const class ImageSequence &sequence);