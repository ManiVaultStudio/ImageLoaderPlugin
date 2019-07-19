#pragma once

#include <QObject>
#include <QSize>
#include <QMap>

#include <QMetaType>

class ImageSequence : public QObject
{
	Q_OBJECT

public:
	ImageSequence();
	ImageSequence(const ImageSequence &other);
	~ImageSequence();

	ImageSequence(const QString &directory, const QSize &imageSize, const QString &imageType);

	QString	directory() const;
	QString	imageType() const;
	QSize	imageSize() const;
	QStringList	imageFilePaths() const;

	void setDirectory(const QString &directory);
	void setImageType(const QString &imageType);
	void setImageSize(const QSize &imageSize);

	void scan();
	void addFile(const QString &imageFilePath);

signals:
	void directoryChanged(const QString &directory);
	void imageTypeChanged(const QString &imageType);
	void imageSizeChanged(const QSize &imageSize);
	void imageFilePathsChanged(const QStringList &imageFilePaths);

private:
	QString			_directory;
	QString			_imageType;
	QSize			_imageSize;
	QStringList		_imageFilePaths;
	bool			_dirty;
};

Q_DECLARE_METATYPE(ImageSequence);

QDebug operator<<(QDebug dbg, const class ImageSequence &sequence);