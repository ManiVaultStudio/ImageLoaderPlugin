#pragma once

#include <memory>

#include <QDialog>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

#include "SequencesTreeModel.h"

namespace Ui {
	class SequenceDialog;
}

class SequenceDialog: public QDialog
{
public:
	SequenceDialog();
	~SequenceDialog();

	void PickDirectory();
	void UpdateImageTree();

private:
	std::unique_ptr<Ui::SequenceDialog> _ui;
	QString								_directory;
	QFileSystemModel					_treeModel;
	QSortFilterProxyModel				_FilteredImagesTreeModel;
};

