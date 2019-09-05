#include "ImageCollection.h"
#include "ImageUtilities.h"

#include <QDebug>

ImageCollection::ImageCollection(const Type& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(ImageCollection::typeName(type))),
	_type(type),
	_imageFilePaths(),
	_resampleImageSettings(&_settings)
{
}

ImageCollection::Type ImageCollection::type() const
{
	return _type;
}

QStringList ImageCollection::imageFilePaths() const
{
	return _imageFilePaths;
}

QString ImageCollection::typeName(const Type& type)
{
	switch (type)
	{
		case Type::Sequence:
			return "Sequence";

		case Type::Stack:
			return "Stack";

		case Type::MultiPartSequence:
			return "MultiPartSequence";

		default:
			break;
	}
	return "";
}