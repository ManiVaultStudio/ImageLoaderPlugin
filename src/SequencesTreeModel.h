#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QFileSystemModel>
#include <QSet>
#include <QSortFilterProxyModel>

#include "ImageSequences.h"

class TreeItem;

/*
class TreeModel : public QFileSystemModel
{
	Q_OBJECT
public:
	TreeModel(QObject* parent = 0);

private:
};
*/

class FilteredImagesTreeModel : public QSortFilterProxyModel
{
public:
	void setExtension(const QString& extension);

protected:
	QString _extension;
	virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit TreeModel(QObject *parent = nullptr);
	~TreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	void scanImages(const QString& rootPath, const QString &imageType, const QSize &size);

private:
	void scanDir(TreeItem *treeItem, const QString &directory);
	void setupModelData(const QStringList &lines, TreeItem *parent);

	QString _rootPath;
	QString _imageType;
	QSize _imageSize;
	TreeItem *rootItem;
	//QMap<QString>
	QStringList _images;
};