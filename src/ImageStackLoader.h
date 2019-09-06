#pragma once

#include "ImageCollectionLoader.h"
#include "ImageStackScanner.h"

#include <QString>

class ImageStackLoader : public ImageCollectionLoader {
	Q_OBJECT

public:
	ImageStackLoader();

	QStringList	dimensionNames() const;
	int noDimensions() const;

	void add(const QString& imageFilePath);

public:

public:
	void load() override;
	
protected:
	void loadImage(const QString& imageFilePath, const int& imageIndex, FloatVector& pointsData);

private:
};