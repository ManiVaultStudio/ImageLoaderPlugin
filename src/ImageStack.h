#pragma once

#include <QThread>
#include <QSize>
#include <QString>

class ImageStack : public QThread {
	Q_OBJECT

public:
	ImageStack(const QSize& size = QSize());

	QSize size() const;
	QStringList	imageFilePaths() const;
	
	int noDimensions() const;
	int noImages() const;
	int noPixels() const;

	void add(const QString& imageFilePath);
	void load();
	
protected:
	void loadImage(const QString& imageFilePath, const int& imageIndex, std::vector<float>& pointsData);

signals:
	void beginLoad(ImageStack* imageStack);
	void endLoad(ImageStack* imageStack, std::vector<float>& pointsData);
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);
	void message(const QString& message);

private:
	QSize			_size;
	QStringList		_imageFilePaths;
};