#pragma once

#include "ImageCollection.h"
#include "ImageStackScan.h"

#include <QString>

class ImageStack : public ImageCollection {
	Q_OBJECT

public:
	ImageStack();

	QStringList	dimensionNames() const;
	int noDimensions() const;

	void add(const QString& imageFilePath);

public:
	void load() override;
	
protected:
	void loadImage(const QString& imageFilePath, const int& imageIndex, FloatVector& pointsData);

private:
	ImageStackScan	_scan;
};