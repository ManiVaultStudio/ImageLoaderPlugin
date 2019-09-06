#pragma once

#include "ImageCollectionLoader.h"

#include <QSize>

class ImageSequenceLoader : public ImageCollectionLoader
{
	Q_OBJECT

public:
	ImageSequenceLoader();

//	QStringList	dimensionNames() const;
//	int noDimensions() const;

public:
	void load();

signals:
	void becameDirty();
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);

private:
	void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData);
};

QDebug operator<<(QDebug dbg, const class ImageSequenceLoader &sequence);