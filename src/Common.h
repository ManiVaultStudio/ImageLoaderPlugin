#pragma once

#include <QModelIndex>

/**
 * Convert enum class type to underlying type
 * @param e Enum
 * @return Underlying type (usually int)
 */
template <typename E>
constexpr auto ult(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

inline bool isColumnInModelIndexRange(const QModelIndex& topLeft, const QModelIndex& bottomRight, const std::uint32_t& column)
{
    return topLeft.column() <= column || bottomRight.column() >= column;
}
