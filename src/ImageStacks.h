#pragma once

#include "ImageStack.h"

#include <QThread>
#include <QSize>
#include <QMap>
#include <QStringList>
#include <QMetaType>

using ImageStackMap = QMap<QString, QSharedPointer<ImageStack>>;

class ImageStacks : public QThread {
	Q_OBJECT

public:
	ImageStacks();
	~ImageStacks() override;

	QString	directory() const;
	QStringList	imageTypes() const;

	ImageStackMap& stacks();
	
	void setDirectory(const QString &directory);
	void setImageTypes(const QStringList &imageTypes);

	void scan();
	
protected:
	void scanDir(const QString &directory);

protected:
	void run() override;

signals:
	void becameDirty();
	void beginScan();
	void endScan();
	void directoryChanged(const QString &directory);

private:
	QString			_directory;
	QStringList		_imageTypes;
	ImageStackMap	_stacks;
};

QDebug operator<<(QDebug dbg, const class ImageStacks &sequence);