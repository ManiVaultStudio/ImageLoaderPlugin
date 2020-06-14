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
	std::int32_t			_flags;				/** Configuration flags */
	QVector<TreeItem*>		_children;
	TreeItem*				m_parentItem;

	friend class ImageCollectionsModel;
};