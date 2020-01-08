#pragma once

#include "ImageData/ImageData.h"
#include "Common.h"

#include <QString>
#include <QSize>
#include <QMap>
#include <QVariant>

/*!
	\class Payload
	\brief Container for loaded images
	\ingroup ImageLoaderPlugin
*/
class Payload
{
public:
	Payload(const ImageCollectionType& type = ImageCollectionType::Undefined);

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

	std::uint32_t noImages() const;

	Image& add(const std::uint32_t& noComponents, const QString& imageFilePath);

protected:
	ImageCollectionType		_type;				/*!< Type of payload e.g. sequence, stack */
	QString					_name;				/*!< Name of the payload */
	QSize					_size;				/*!< Size of the images */
	std::vector<Image>		_images;			/*!< Loaded images */
	QStringList				_imageFilePaths;	/*!< File paths of the loaded images */
};

QDebug operator<<(QDebug dbg, Payload& payload);