/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#ifndef FORMATTER_H
#define FORMATTER_H

#include "gt_loglevel.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace gt
{
namespace log
{

struct Details
{
    std::string id;
    std::tm time;
};

/**
 * @brief Formats the time struct depending on the format string. The time may
 * be formatted using the identifiers of std::put_string.
 *
 * Example:
 *
 * gt::log::formatTime(<time>, "[%H:%M:%S]")
 * // out: "[11::12:42]"
 *
 * @param time Time
 * @param format Format string
 * @return formatted string
 */
inline std::string
formatTime(std::tm const& time, char const* format = "%H:%M:%S")
{
    std::ostringstream s;
    s << std::put_time(&time, format);
    return s.str();
}

namespace detail
{

//! to string helper methods
inline std::string const& toString(std::string const& s) { return s; }
inline std::string toString(Level level) { return levelToString(level); }
inline std::string toString(std::tm time) { return formatTime(time); }

//! No more args to format
template <typename FormatIterA>
inline std::string
formatImpl(size_t /*idx*/, FormatIterA begin, FormatIterA end)
{
    // nothing to do here
    return std::string{begin, end};
}

//! Format arg
template <typename Iter, typename Arg, typename... Args>
inline std::string
formatImpl(size_t idx, Iter begin, Iter end, Arg const& arg, Args const&... args)
{
    using CharT = typename std::iterator_traits<Iter>::value_type;

    // argument index in range from 1-9 (e.g. %2, %3)
    CharT ident[] = "%1";
    constexpr size_t identlen = sizeof(ident) - 1;
    ident[1] += static_cast<char>(idx);

    // search for the identifier
    auto iter = std::search(begin, end, ident, ident + identlen);

    if (iter != end)
    {

        return
            // format pending args in the string until this match
            // as the next identifier may be in front of this match
            formatImpl(idx + 1, begin, iter, args...) +
            // format argument
            toString(arg) +
            // format pending args in the string after this match
            formatImpl(idx + 1, iter + identlen, end, args...);
    }

    // no match -> skip arg and format remaining string
    return formatImpl(idx + 1, begin, end, args...);
}

} // namespace details

/**
 * @brief Formats the argument list depending on the format string similar to
 * printf. Arguments will be inserted in the format using identifieres in the
 * form of "%<n>" where <n> denotes an argument in the range of 1 - 9. Unmatched
 * arguments will be skipped.
 *
 * Example:
 *
 * gt::log::format("%1 - %2", "my_id", "my message")
 * // out: "my_id - my message"
 *
 * @param format Format string.
 * @param args Arguments to insert in the format string
 * @return formatted string
 */
template <typename... Args>
inline std::string
format(std::string const& format, Args const&... args)
{
    return detail::formatImpl(0ul, format.begin(), format.end(), args...);
}

//! Helper class for formatting a log message
class Formatter
{
public:

    //! Default ctor
    Formatter() = default;

    //! ctor accepting a custom format and level filter
    explicit Formatter(std::string format,
                       int filter = -1,
                       std::string timeFormat = defaultTimeFormat())
        : m_format(std::move(format))
        , m_timeFormat(std::move(timeFormat))
        , m_filter(filter)
    { }

    //! Returns whether this level should be logged
    bool filter(gt::log::Level level)
    {
        return m_filter & levelToInt(level);
    }

    //! Sets the filter level
    Formatter& filterLevel(gt::log::Level level, bool include = true)
    {
        if (include)
        {
            m_filter |=  levelToInt(level);
        }
        else
        {
            m_filter &= ~levelToInt(level);
        }
        return *this;
    }

    //! Whether to include all levels (if true) else excludes all levels
    Formatter& filterAll(bool include = true)
    {
        m_filter &= include ? -1 : 0;
        return *this;
    }

    //! Setter for the format
    Formatter& setFormat(std::string format)
    {
        m_format = std::move(format);
        return *this;
    }

    //! Default format
    static std::string const& defaultFormat()
    {
        static const std::string fmt = "%2 [%3] [%4] %1";
        return fmt;
    }

    //! Default format
    static std::string const& defaultTimeFormat()
    {
        static const std::string fmt = "%H:%M:%S";
        return fmt;
    }

    //! Formats the arguments into a single string acording to the format.
    std::string format(std::string const& message,
                       Level level,
                       Details const& details) const
    {
        return gt::log::format(m_format, message, level, details.id,
                               formatTime(details.time, m_timeFormat.c_str()));
    }

    //! Getter for format string
    std::string const& format() const
    {
        return m_format;
    }

private:

    /**
     * %1 = Message
     * %2 = Level
     * %3 = Id
     * %4 = Time
     */
    std::string m_format{defaultFormat()};

    std::string m_timeFormat{defaultTimeFormat()};

    /// Bitfield to filter out only certain levels (by default all levels)
    int m_filter{-1};
};

} // end namespace log

} // end namespace gt

#endif // FORMATTER_H
