#include "SequencesTreeModel.h"
#include "TreeItem.h"

#include <QDebug>
#include <QStringList>
#include <QDirIterator>
#include <QImageReader>
#include <QSet>

TreeModel::TreeModel(QObject* parent) :
	QFileSystemModel(parent)
{
}

/*
bool FilteredImagesTreeModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);

	QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
	QFileSystemModel* fileModel = qobject_cast<QFileSystemModel*>(sourceModel());

	if (fileModel->isDir(index0)) {
		qDebug() << fileModel->fileName(index0) << fileModel->hasChildren(index0) << fileModel->rowCount(index0);
		return fileModel->hasChildren(index0);
	}
	
	return true;
}
*/

bool FilteredImagesTreeModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
	

	// custom behaviour :
	if (filterRegExp().isEmpty() == false)
	{
		// get source-model index for current row
		QModelIndex source_index = sourceModel()->index(source_row, this->filterKeyColumn(), source_parent);
		if (source_index.isValid())
		{
			// if any of children matches the filter, then current index matches the filter as well
			int i, nb = sourceModel()->rowCount(source_index);
			for (i = 0; i < nb; ++i)
			{
				if (filterAcceptsRow(i, source_index))
				{
					return true;
				}
			}
			// check current index itself :
			QString key = sourceModel()->data(source_index, filterRole()).toString();
			return key.contains(filterRegExp());
		}
	}
	// parent call for initial behaviour
	return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}
