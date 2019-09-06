#pragma once

#include "ImageCollectionLoader.h"
#include "ImageStackScanner.h"

#include <QString>

class ImageStack : public ImageCollectionLoader {
	Q_OBJECT

public:
	ImageStack();

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