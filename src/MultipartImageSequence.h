#pragma once

#include <QObject>
#include <QSize>
#include <QMap>
#include <QMetaType>

class MultipartImageSequence : public QObject
{
	Q_OBJECT

public:
	MultipartImageSequence();
	MultipartImageSequence(const MultipartImageSequence &other);
	~MultipartImageSequence();

	QString	directory() const;
	QStringList	imageFilePaths() const;
	QStringList	dimensionNames() const;
	std::vector<float>& pointsData();
	int noDimensions() const;
	int noImages() const;
	void setDirectory(const QString &directory);

protected:
	void scanDir(const QString &directory);
	void addFile(const QString &imageFilePath);
	void loadImage(const QString & imageFilePath);

signals:
	void becameDirty();
	void directoryChanged(const QString &directory);
	void imageFilePathsChanged(const QStringList &imageFilePaths);
	void beginLoad();
	void endLoad();
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);

private:
	QString				_directory;
	QStringList			_imageFilePaths;
	std::vector<float>	_pointsData;
};

Q_DECLARE_METATYPE(MultipartImageSequence);

QDebug operator<<(QDebug dbg, const class ImageSequence &sequence);