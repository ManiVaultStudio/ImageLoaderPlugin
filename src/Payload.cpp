#include "Payload.h"

#include <QDebug>

Payload::Payload(const ImageCollectionType& type /*= ImageCollectionType::Undefined*/) :
	_type(type),
	_name(),
	_images(),
	_imageFilePaths()
{
}

ImageCollectionType Payload::type() const
{
	return _type;
}

void Payload::setType(const ImageCollectionType& type)
{
	_type = type;
}

QString Payload::name() const
{
	return _name;
}

void Payload::setName(const QString& name)
{
	_name = name;
}

QSize Payload::size() const
{
	return _size;
}

void Payload::setSize(const QSize& size)
{
	_size = size;
}

std::vector<Image>& Payload::images()
{
	return _images;
}

QStringList Payload::imageFilePaths() const
{
	return _imageFilePaths;
}

void Payload::setImageFilePaths(const QStringList& imageFilePaths)
{
	_imageFilePaths = imageFilePaths;
}

std::uint32_t Payload::noImages() const
{
	return static_cast<std::uint32_t>(_images.size());
}

Image& Payload::add(const std::uint32_t& noComponents, const QString& imageFilePath)
{
	_images.push_back(Image(_size, noComponents, imageFilePath));

	return _images.back();
}

QDebug operator<<(QDebug dbg, Payload & payload)
{
	const auto noImages = QString::number(payload.noImages());

	dbg << QString("Image dataset '%1' of type %2, %3 images").arg(payload.name(), imageCollectionTypeName(payload.type()), noImages);

	return dbg;
}