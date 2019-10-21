#include "Images.h"

#include <QDebug>

Images::Images(const ImageCollectionType& type) :
	_type(type),
	_name(),
	_images(),
	_imageFilePaths(),
	_dimensionNames()
{
}

ImageCollectionType Images::type() const
{
	return _type;
}

void Images::setType(const ImageCollectionType& type)
{
	_type = type;
}

QString Images::name() const
{
	return _name;
}

void Images::setName(const QString& name)
{
	_name = name;
}

QSize Images::size() const
{
	return _size;
}

void Images::setSize(const QSize& size)
{
	_size = size;
}

std::vector<Image>& Images::images()
{
	return _images;
}

QStringList Images::imageFilePaths() const
{
	return _imageFilePaths;
}

void Images::setImageFilePaths(const QStringList& imageFilePaths)
{
	_imageFilePaths = imageFilePaths;
}

std::uint32_t Images::noImages() const
{
	return static_cast<std::uint32_t>(_images.size());
}

Image& Images::add(const std::uint32_t& noComponents, const QString& imageFilePath)
{
	_images.push_back(Image(_size, noComponents, imageFilePath));

	return _images.back();
}

QStringList Images::dimensionNames() const
{
	return _dimensionNames;
}

void Images::setDimensionNames(const QStringList & dimensionNames)
{
	_dimensionNames = dimensionNames;
}

QDebug operator<<(QDebug dbg, Images & imageDataSet)
{
	const auto noImages = QString::number(imageDataSet.noImages());

	dbg << QString("Image dataset '%1' of type %2, %3 images").arg(imageDataSet.name(), imageCollectionTypeName(imageDataSet.type()), noImages);

	return dbg;
}
