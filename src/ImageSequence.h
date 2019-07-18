#pragma once

#include <QSize>
#include <QMap>

#include <QMetaType>

class ImageSequence
{
public:
	ImageSequence();
	ImageSequence(const ImageSequence &other);
	~ImageSequence();

	ImageSequence(const QSize &size, const QString &type, const QString &directory);

	QSize	size() const;
	QString	type() const;
	QString	directory() const;
	QStringList	files() const;

	void addFile(const QString &imageFilePath);

private:
	QSize		_size;
	QString		_type;
	QString		_directory;
	QStringList	_files;
};

Q_DECLARE_METATYPE(ImageSequence);

QDebug operator<<(QDebug dbg, const class ImageSequence &sequence);