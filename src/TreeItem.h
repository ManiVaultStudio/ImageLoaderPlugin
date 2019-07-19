#pragma once

#include <QVariant>
#include <QVector>

class TreeItem
{
public:
	explicit TreeItem(const QVector<QVariant> &data, TreeItem *parentItem = nullptr);
	~TreeItem();

	void appendChild(TreeItem *child);

	TreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	TreeItem *parentItem();
	void removeChildren();

private:
	QVector<TreeItem*> m_childItems;
	QVector<QVariant> m_itemData;
	TreeItem *m_parentItem;
};