#pragma once

#include <QThread>
#include <QSize>
#include <QMap>

#include <QMetaType>

class ImageStack : public QThread
{
	Q_OBJECT

public:
	ImageStack();
	ImageStack(const ImageStack &other);
	~ImageStack();

	enum RunMode
	{
		Scan,
		Load
	};

	RunMode	runMode() const;
	QString	directory() const;
	QStringList	imageTypes() const;
	QSize	imageSize() const;
	QStringList	imageFilePaths() const;
	std::vector<float>& pointsData();
	int noDimenions() const;

	void setRunMode(const RunMode &runMode);
	void setDirectory(const QString &directory);
	void setImageTypes(const QStringList &imageTypes);
	void setImageSize(const QSize &imageSize);

	void scan();
	
protected:
	void scanDir(const QString &directory);
	void addFile(const QString &imageFilePath);
	void loadImage(const QString & imageFilePath);

protected:
	void run() override;

signals:
	void becameDirty();
	void beginScan();
	void endScan();
	void message(const QString& message);
	void directoryChanged(const QString &directory);
	void imageSizeChanged(const QSize &imageSize);
	void imageFilePathsChanged(const QStringList &imageFilePaths);
	void beginLoad();
	void endLoad();
	void imageLoaded(const QString &imageFilePath, const int &done, const int &total);

private:
	RunMode				_runMode;
	QString				_directory;
	QStringList			_imageTypes;
	QSize				_imageSize;
	QStringList			_imageFilePaths;
	std::vector<float>	_pointsData;
};

Q_DECLARE_METATYPE(ImageStack);

QDebug operator<<(QDebug dbg, const class ImageStack &sequence);