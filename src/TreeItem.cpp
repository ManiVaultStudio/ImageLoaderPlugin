#include "TreeItem.h"

TreeItem::TreeItem(TreeItem *parent) :
	m_parentItem(parent)
{
}

TreeItem::~TreeItem()
{
	qDeleteAll(_children);
}

void TreeItem::appendChild(TreeItem *item)
{
	_children.append(item);
}

TreeItem *TreeItem::child(int row)
{
	if (row < 0 || row >= _children.size())
		return nullptr;
	return _children.at(row);
}

int TreeItem::childCount() const
{
	return _children.count();
}

int TreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->_children.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

TreeItem *TreeItem::parentItem()
{
	return m_parentItem;
}