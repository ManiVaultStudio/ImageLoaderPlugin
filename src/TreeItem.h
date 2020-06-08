#pragma once

#include <QVector>
#include <QVariant>

class TreeItem
{
public:
	explicit TreeItem(TreeItem *parentItem = nullptr);
	~TreeItem();

	void appendChild(TreeItem *child);

	TreeItem *child(int row);
	int childCount() const;
	QVariant data(int column) const;
	int row() const;
	TreeItem *parentItem();

protected:
	QVector<TreeItem*> m_childItems;
	TreeItem *m_parentItem;
};