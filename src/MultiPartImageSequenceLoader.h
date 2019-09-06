#pragma once

#include "ImageCollectionLoader.h"

#include <QSize>
#include <QMap>

class MultiPartImageSequenceLoader : public ImageCollectionLoader
{
	Q_OBJECT

public:
	MultiPartImageSequenceLoader();

	QStringList	dimensionNames() const;
	std::vector<float>& pointsData();
	int noDimensions() const;

public:
	void load() override;

protected:
	void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData);
	
signals:
	void becameDirty();
	void imageFilePathsChanged(const QStringList &imageFilePaths);

private:
	QString				_directory;
	std::vector<float>	_pointsData;
};

QDebug operator<<(QDebug dbg, const class ImageSequence &sequence);