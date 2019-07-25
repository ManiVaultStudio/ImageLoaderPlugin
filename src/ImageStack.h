#pragma once

#include <QThread>
#include <QSize>
#include <QString>

class ImageStack : public QThread {
	Q_OBJECT

public:
	ImageStack(const QSize& size = QSize());
	ImageStack(const ImageStack &other);
	~ImageStack();

	ImageStack& operator = (const ImageStack& other)
	{
		return ImageStack(other);
	}

	QStringList	imageFilePaths() const;
	
	int noDimensions() const;

	void add(const QString& imageFilePath);
	void load();
	
protected:
	void loadImage(const QString& imageFilePath, std::vector<float>& pointsData);

signals:
	void beginLoad(ImageStack* imageStack);
	void endLoad(ImageStack* imageStack, std::vector<float>& pointsData);
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);

private:
	QSize			_size;
	QStringList		_imageFilePaths;
};