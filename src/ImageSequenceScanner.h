#pragma once

#include "ImageScanner.h"

#include <QString>
#include <QSize>

class QSettings;

/*!
	\class ImageSequenceScanner
	\inherits ImageScanner
	\brief Scans for image sequence
	\ingroup ImageLoaderPlugin
*/
class ImageSequenceScanner : public ImageScanner {
	Q_OBJECT

public:
	ImageSequenceScanner();

	void loadSettings() override;

public:
	QString	imageType() const;
	void setImageType(const QString &imageType, const bool& forceUpdate = false);
	QSize imageSize() const;
	void setImageWidth(const std::uint32_t& imageWidth, const bool& forceUpdate = false);
	void setImageHeight(const std::uint32_t& imageHeight, const bool& forceUpdate = false);
	void setImageSize(const QSize &imageSize, const bool& forceUpdate = false);
	bool square() const;
	void setSquare(const bool& square, const bool& forceUpdate = false);

public:
	void scan() override;
	void run() override;

private:
	void scanDir(const QString& directory, ImageCollection& imageCollection);

signals:
	void imageTypeChanged(const QString& imageType);
	void imageSizeChanged(const QSize& imageSize);
	void squareChanged(const bool& square);

private:
	QString		_imageType;		/*!< Type of image e.g. tiff, jpg */
	QSize		_imageSize;		/*!< Two-dimensional size of the image */
	bool		_square;		/*!< Whether the image(s) are square */
};