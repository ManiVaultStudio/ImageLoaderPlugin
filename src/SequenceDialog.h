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
	void onMessage(const QString &message);
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);
	void onScan();
	void onPickDirectory();
	void onImageTypeChanged(const QString &imageType);
	void onBeginLoad();
	void onEndLoad();

private:
	ImageLoader*							_imageLoaderPlugin;
	std::unique_ptr<Ui::SequenceDialog>		_ui;
	TreeModel								_treeModel;
	ImageSequence							_imageSequence;
};

