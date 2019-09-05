#include "ImageCollection.h"
#include "ImageUtilities.h"
#include "ImageScanner.h"

#include <QDebug>

ImageCollection::ImageCollection(const Type& type) :
	_settings("HDPS", QString("Plugins/ImageLoader/%1").arg(ImageCollection::typeName(type))),
	_type(type),
	_imageFilePaths(),
	_resampleImageSettings(&_settings),
	_scanner(nullptr)
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

int ImageCollection::noImages() const
{
	return _imageFilePaths.size();
}

ResampleImageSettings & ImageCollection::resampleImageSettings()
{
	return _resampleImageSettings;
}

void ImageCollection::reset()
{
	_imageFilePaths.clear();
}

void ImageCollection::setScanner(std::unique_ptr<ImageScanner>& scanner)
{
	_scanner.swap(scanner);

	connect(_scanner.get(), &ImageScanner::message, this, &ImageCollection::message);
}

QVariant ImageCollection::setting(const QString& name, const QVariant& defaultValue) const
{
	return _settings.value(name, defaultValue).toString();
}

void ImageCollection::setSetting(const QString& name, const QVariant& value)
{
	_settings.setValue(name, value);
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