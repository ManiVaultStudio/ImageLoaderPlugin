#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QFileSystemModel>
#include <QSet>
#include <QSortFilterProxyModel>

#include "ImageSequences.h"

class TreeItem;

class TreeModel : public QFileSystemModel
{
	Q_OBJECT
public:
	TreeModel(QObject* parent = 0);

private:
};

class FilteredImagesTreeModel : public QSortFilterProxyModel
{
protected:
	virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};