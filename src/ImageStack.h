#pragma once

#include "ImageCollection.h"

#include <QSize>
#include <QString>

class ImageStack : public ImageCollection {
	Q_OBJECT

public:
	ImageStack();

	QString	directory() const;
	void setDirectory(const QString &directory);
	QStringList	imageTypes() const;
	void setImageTypes(const QStringList &imageTypes);

	QStringList	dimensionNames() const;
	
	int noDimensions() const;

	void add(const QString& imageFilePath);

public:
	void scan();
	void scanDir(const QString & directory);
	void load() override;
	
protected:
	void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData);

signals:
	void becameDirty();
	void beginScan();
	void endScan();
	void directoryChanged(const QString& directory);

private:
	QString			_directory;
	QStringList		_imageTypes;
};