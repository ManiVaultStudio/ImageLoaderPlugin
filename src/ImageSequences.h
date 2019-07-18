#pragma once

#include <QSize>
#include <QMap>

#include <QMetaType>

class ImageSequences
{
public:
	ImageSequences();
	ImageSequences(const ImageSequences &other);
	~ImageSequences();

	QString	directory() const;

	void load(const QString &directory);

private:
	QString		_directory;
};

Q_DECLARE_METATYPE(ImageSequences);

QDebug operator<<(QDebug dbg, const class ImageSequences &sequences);