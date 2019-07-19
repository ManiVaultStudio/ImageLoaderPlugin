#include "SequencesTreeModel.h"
#include "TreeItem.h"

#include <QDebug>
#include <QStringList>
#include <QDirIterator>
#include <QImageReader>
#include <QSet>

TreeModel::TreeModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	rootItem = new TreeItem({ tr("Name"), tr("Type") });
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void TreeModel::scanImages(const QString& rootPath, const QString &imageType, const QSize &imageSize)
{
	_rootPath = rootPath;
	_imageType = imageType;
	_imageSize = imageSize;

	qDebug() << "Scanning: " << rootPath;

	/*
	QStringList nameFilter("*." + _imageType);
	QDir directory(rootPath);
	QStringList txtimageFilesAndDirectories = directory.entryList(nameFilter);
	

	QDirIterator it(rootPath, QStringList() << "*." + _imageType, QDir::imageFiles, QDirIterator::Subdirectories);

	
	while (it.hasNext())
		qDebug() << it.next();
		*/

	_images.clear();

	// qDebug() << nameFilter << txtimageFilesAndDirectories;
	scanDir(rootItem, _rootPath);

	qDebug() << "Found " << _images.size() << " images";
}

void TreeModel::scanDir(TreeItem *parentTreeItem, const QString &directory)
{
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		QVector<QVariant> columnData;

		columnData << dirList.at(i) << "File folder";

		auto dirTreeItem = new TreeItem(columnData, parentTreeItem);

		parentTreeItem->appendChild(dirTreeItem);

		scanDir(dirTreeItem, path);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);
	imageFiles.setNameFilters(QStringList() << "*." + _imageType);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		if (imageReader.size() == _imageSize) {
			// qDebug() << "Found image: " << fileList.at(i);

			QVector<QVariant> columnData;

			columnData << fileList.at(i) << _imageType + " file";

			auto fileTreeItem = new TreeItem(columnData, parentTreeItem);

			parentTreeItem->appendChild(fileTreeItem);

			_images.append(path);

			scanDir(fileTreeItem, path);
		}
	}
}