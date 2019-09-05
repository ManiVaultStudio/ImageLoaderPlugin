#pragma once

#include "ImageStack.h"
#include "ImageCollection.h"

#include <QSize>
#include <QMap>
#include <QStringList>
#include <QMetaType>

using ImageStackMap = QMap<QString, QSharedPointer<ImageStack>>;

class ImageStacks : public ImageCollection {
	Q_OBJECT

public:
	ImageStacks();

	QString	directory() const;
	QStringList	imageTypes() const;

	ImageStackMap& stacks();
	
	void setDirectory(const QString &directory);
	void setImageTypes(const QStringList &imageTypes);

	void scan();
	void load();

protected:
	void scanDir(const QString &directory);


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