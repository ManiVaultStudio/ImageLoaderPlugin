#include "Payload.h"

#include <QDebug>

Payload::Payload(const ImageData::Type& type /*= ImageData::Type::Undefined*/) :
	_type(type),
	_name(),
	_images(),
	_imageFilePaths()
{
}

ImageData::Type Payload::type() const
{
	return _type;
}

void Payload::setType(const ImageData::Type& type)
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

	if (_size.width() == 0)
		_size.setWidth(1);

	if (_size.height() == 0)
		_size.setHeight(1);
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

Image& Payload::add(const std::uint32_t& noComponentsPerPixel, const QString& imageFilePath)
{
	_images.push_back(Image(_size, noComponentsPerPixel, imageFilePath));

	return _images.back();
}

QDebug operator<<(QDebug dbg, Payload & payload)
{
	const auto noImages = QString::number(payload.noImages());

	dbg << QString("Image dataset '%1' of type %2, %3 images").arg(payload.name(), ImageData::typeName(payload.type()), noImages);

	return dbg;
}