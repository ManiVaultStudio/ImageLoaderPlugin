#pragma once

#include "ImageCollection.h"

#include <QSize>
#include <QString>

class ImageStack : public ImageCollection {
	Q_OBJECT

public:
	ImageStack();

	QSize size() const;
	QStringList	dimensionNames() const;
	
	int noDimensions() const;
	int noPixels() const;

	void add(const QString& imageFilePath);

public:
	void scan();
	void load() override;
	
protected:
	void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData);

private:
	QSize			_size;
};