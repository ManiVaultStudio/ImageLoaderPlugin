#pragma once

#include <QThread>
#include <QSize>
#include <QMap>
#include <QStringList>
#include <QMetaType>

class ImageStacks : public QThread {
	Q_OBJECT

public:
	ImageStacks();
	ImageStacks(const ImageStacks &other);
	~ImageStacks();

	enum RunMode {
		Scan,
		Load
	};

	RunMode	runMode() const;
	QString	directory() const;
	QStringList	imageTypes() const;
	QStringList	imageFilePaths() const;
	QMap<QString, QStringList> stacks() const;
	std::vector<float>& pointsData();
	int noDimenions() const;

	void setRunMode(const RunMode &runMode);
	void setDirectory(const QString &directory);
	void setImageTypes(const QStringList &imageTypes);

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
	RunMode						_runMode;
	QString						_directory;
	QStringList					_imageTypes;
	QStringList					_imageFilePaths;
	QMap<QSize, Stack>			_stacks;
	std::vector<float>			_pointsData;
};

Q_DECLARE_METATYPE(ImageStacks);

QDebug operator<<(QDebug dbg, const class ImageStacks &sequence);