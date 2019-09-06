#include "ImageCollectionLoader.h"
#include "ImageUtilities.h"
#include "ImageScanner.h"

#include <QDebug>

ImageCollectionLoader::ImageCollectionLoader(const ImageCollectionType& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(ImageCollectionLoader::typeName(type))),
	_type(type),
	_imageFilePaths(),
	_resampleImageSettings(&_settings)
{
}

ImageCollectionType ImageCollectionLoader::type() const
{
	return _type;
}

QStringList ImageCollectionLoader::imageFilePaths() const
{
	return _imageFilePaths;
}

int ImageCollectionLoader::noImages() const
{
	return _imageFilePaths.size();
}

ResampleImageSettings & ImageCollectionLoader::resampleImageSettings()
{
	return _resampleImageSettings;
}

void ImageCollectionLoader::reset()
{
	_imageFilePaths.clear();
}

QVariant ImageCollectionLoader::setting(const QString& name, const QVariant& defaultValue) const
{
	return _settings.value(name, defaultValue).toString();
}

void ImageCollectionLoader::setSetting(const QString& name, const QVariant& value)
{
	_settings.setValue(name, value);
}

QString ImageCollectionLoader::typeName(const ImageCollectionType& type)
{
	switch (type)
	{
		case ImageCollectionType::Sequence:
			return "Sequence";

		case ImageCollectionType::Stack:
			return "Stack";

		case ImageCollectionType::MultiPartSequence:
			return "MultiPartSequence";

		default:
			break;
	}
	return "";
}