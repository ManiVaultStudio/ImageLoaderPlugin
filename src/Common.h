#pragma once

template <typename E>
constexpr auto ult(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}