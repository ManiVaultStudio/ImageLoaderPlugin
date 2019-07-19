#pragma once

#include <memory>

#include <QDialog>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

#include "SequencesTreeModel.h"
#include "ImageSequence.h"

namespace Ui {
	class SequenceDialog;
}

class ImageLoader;

class SequenceDialog: public QDialog
{
public:
	SequenceDialog(ImageLoader *imageLoader);
	~SequenceDialog();

	void PickDirectory();
	void Scan();

protected:
	void ScanDirectoryForImages(const QString& rootPath, const QString &imageType, const QSize &size);
	void ScanDir(const QString &directory);

private:
	ImageLoader*							_imageLoaderPlugin;
	std::unique_ptr<Ui::SequenceDialog>		_ui;
	TreeModel								_treeModel;
	ImageSequence							_imageSequence;
};

