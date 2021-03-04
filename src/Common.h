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

/**
 * Returns a human readable string of an integer counter
 * @param count Integer count
 * @return Human readable string of an integer counter
 */
static QString getIntegerCountHumanReadable(const int& count)
{
    if (count >= 0 && count < 1000)
        return QString::number(count);

    if (count >= 1000 && count < 1000000)
        return QString("%1 k").arg(QString::number(count / 1000.0f, 'f', 1));

    if (count >= 1000000)
        return QString("%1 mln").arg(QString::number(count / 1000000.0f, 'f', 1));

    return "";
}

/**
 * Returns a human readable string of a byte counter
 * @param noBytes Number of bytes
 * @return Human readable string of a byte counter
 */
static QString getNoBytesHumanReadable(std::uint32_t noBytes)
{
    QStringList list{ "KB", "MB", "GB", "TB" };

    QStringListIterator it(list);
    QString unit("bytes");

    while (noBytes >= 1024.0 && it.hasNext())
    {
        unit = it.next();
        noBytes /= 1024.0;
    }

    return QString::number(noBytes, 'f', 2) + " " + unit;
}