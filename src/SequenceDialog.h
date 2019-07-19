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

private slots:
	void onBecameDirty();
	void onBeginScan();
	void onEndScan();
	void onFoundImageFile(const QString &imageFilePath);
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);

private:
	ImageLoader*							_imageLoaderPlugin;
	std::unique_ptr<Ui::SequenceDialog>		_ui;
	TreeModel								_treeModel;
	ImageSequence							_imageSequence;
};

