#pragma once

#include <QString>

template <typename E>
constexpr auto ult(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}

static QString formatIntegerCount(const int& count) {
	if (count >= 0 && count < 1000)
		return QString::number(count);

	if (count >= 1000 && count < 100000)
		return QString("%1 k").arg(QString::number(count / 1000.0f, 'f', 1));

	if (count >= 100000)
		return QString("%1 mln").arg(QString::number(count / 100000.0f, 'f', 1));

	return "";
}