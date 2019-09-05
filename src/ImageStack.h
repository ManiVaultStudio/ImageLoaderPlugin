#pragma once

#include "ImageCollection.h"
#include "ImageStackScanner.h"

#include <QString>

class ImageStack : public ImageCollection {
	Q_OBJECT

public:
	ImageStack();

	QStringList	dimensionNames() const;
	int noDimensions() const;

	void add(const QString& imageFilePath);

	ImageStackScanner& scanner();

public:
	void load() override;
	
protected:
	void loadImage(const QString& imageFilePath, const int& imageIndex, FloatVector& pointsData);

private:
	ImageStackScanner	_scanner;
};