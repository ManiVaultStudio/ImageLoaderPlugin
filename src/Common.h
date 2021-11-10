#pragma once

#include <QStringList>

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
