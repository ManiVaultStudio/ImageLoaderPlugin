#include "TreeItem.h"

TreeItem::TreeItem(TreeItem *parent) :
    _flags(0),
    _children(),
    _parentItem(parent)
{
}

TreeItem::~TreeItem()
{
    removeAllChildren();
}

void TreeItem::appendChild(TreeItem* child)
{
    _children.append(child);
}

TreeItem* TreeItem::child(int row)
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
    if (_parentItem)
        return _parentItem->_children.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

TreeItem* TreeItem::getParentItem()
{
    return _parentItem;
}

void TreeItem::removeAllChildren()
{
    qDeleteAll(_children);
    _children.clear();
}

bool TreeItem::isFlagSet(const std::int32_t& flag) const
{
    return _flags & static_cast<int>(flag);
}

void TreeItem::setFlag(const std::int32_t& flag, const bool& enabled /*= true*/)
{
    if (enabled)
        _flags |= static_cast<int>(flag);
    else
        _flags = _flags & ~static_cast<int>(flag);
}

std::int32_t TreeItem::flags(const int& role) const
{
    return _flags;
}

void TreeItem::setFlags(const int& flags)
{
    _flags = flags;
}