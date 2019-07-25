#pragma once

#include <QThread>
#include <QSize>
#include <QMap>
#include <QAbstractListModel>

#include <QMetaType>

class Layer {
public:
	QString	imageName() const;
};

class ImageStackModel : public QAbstractListModel {

	Q_OBJECT

public:
	explicit ImageStackModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

private:
	std::vector<Layer*> _layers;
};

class ImageStack : public QThread {
	Q_OBJECT

public:
	ImageStack();
	ImageStack(const ImageStack &other);
	~ImageStack();

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
	ImageStackModel& model();
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
	QMap<QString, QStringList>	_stacks;
	std::vector<float>			_pointsData;
	ImageStackModel				_model;
};

Q_DECLARE_METATYPE(ImageStack);

QDebug operator<<(QDebug dbg, const class ImageStack &sequence);