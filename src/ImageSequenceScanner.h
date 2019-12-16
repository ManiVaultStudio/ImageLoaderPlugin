#pragma once

#include "ImageScanner.h"

#include <QString>
#include <QSize>

class QSettings;

class ImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	ImageSequenceScanner();

public:
	QString	imageType() const;
	void setImageType(const QString &imageType);
	QSize imageSize() const;
	void setImageSize(const QSize &imageSize);

public:
	void scan();
	void run() override;
	void scanDir(const QString& directory, ImageCollection& imageCollection);

signals:
	void imageTypeChanged(const QString& imageType);
	void imageSizeChanged(const QSize& imageSize);

private:
	QString		_imageType;
	QSize		_imageSize;
};