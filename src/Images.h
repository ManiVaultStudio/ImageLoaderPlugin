#pragma once

#include "ImageData/ImageData.h"
#include "Common.h"

#include <QString>
#include <QSize>
#include <QMap>
#include <QVariant>

class Images {
public:
	Images(const ImageCollectionType& type);

public:
	ImageCollectionType type() const;
	void setType(const ImageCollectionType& type);
	QString name() const;
	void setName(const QString& name);
	QSize size() const;
	void setSize(const QSize& size);
	std::vector<Image>& images();
	QStringList imageFilePaths() const;
	void setImageFilePaths(const QStringList& imageFilePaths);
	QStringList dimensionNames() const;
	void setDimensionNames(const QStringList& dimensionNames);

	std::uint32_t noImages() const;

	Image& add(const std::uint32_t& noComponents, const QString& imageFilePath);

protected:
	ImageCollectionType		_type;
	QString					_name;
	QSize					_size;
	std::uint32_t			_noComponents;
	std::vector<Image>		_images;
	QStringList				_imageFilePaths;
	QStringList				_dimensionNames;
};

QDebug operator<<(QDebug dbg, Images& imageDataSet);