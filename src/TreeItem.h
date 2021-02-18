#pragma once

#include <QVector>
#include <QVariant>

/**
 * Tree item class
 *
 * Generic tree item class
 *
 * @author Thomas Kroes
 */
class TreeItem
{
public:
    /**
     * Constructor
     * @param parentItem Pointer to parent tree item
     */
    explicit TreeItem(TreeItem *parentItem = nullptr);

    /** Destructor */
    ~TreeItem();

    /**
     * Append a child
     * @param child Child to append
     */
    void appendChild(TreeItem* child);

    /**
     * Get child tree item by \p row index
     * @param row Row index
     * @return Child tree item
     */
    TreeItem* child(int row);

    /** Returns the number of children */
    int childCount() const;

    /**
     * Get data belonging to \p column
     * @return Data in variant form
     */
    QVariant data(int column) const;

    /** Returns the row index (relative to parent) */
    int row() const;

    /** Returns the parent item */
    TreeItem* getParentItem();

    /** Removes all children */
    void removeAllChildren();

    /**
     * Returns whether specified configuration flag is set or not
     * @param flag Configuration flag
     * @return Whether the configuration flag is set or not
     */
    bool isFlagSet(const std::int32_t& flag) const;

    /**
     * Sets the node configuration flag
     * @param flag Configuration flag
     * @param enabled Whether to enable/disable the configuration flag
     */
    void setFlag(const std::int32_t& flag, const bool& enabled = true);

    /**
     * Returns the configuration flags
     * @param role Data role
     * @return Configuration flags
     */
    std::int32_t flags(const int& role) const;

    /**
     * Sets the node configuration flags
     * @param flags Configuration flags
     */
    void setFlags(const int& flags);

protected:
    std::int32_t            _flags;         /** Configuration flags */
    QVector<TreeItem*>      _children;      /** Vector of pointer to child tree items */
    TreeItem*               _parentItem;    /** Pointer to parent item (nullptr if root) */

    friend class ImageCollectionsModel;
};