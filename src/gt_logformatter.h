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

#include <functional>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cassert>

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
template <typename Iter>
inline std::string
formatImpl(size_t /*idx*/, Iter begin, Iter end)
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

    /// Logging funcion for detailed messages
    using Functor =
            std::function<std::string(std::string const&, Level, Details const&)>;

    /// Default formatter functor
    struct Default
    {
        std::string operator()(std::string const& msg, Level lvl, Details const& dts) const noexcept;
    };

    //! Default ctor
    Formatter() = default;

    //! ctor accepting a custom format and level filter
    explicit Formatter(Functor functor)
        : m_functor(std::move(functor))
    {
        assert(m_functor);
    }

    //! Formats the arguments into a single string acording to the format.
    std::string format(std::string const& message,
                       Level level,
                       Details const& details) const
    {
        return m_functor(message, level, details);
    }

    //! Setter for the format functor
    void setFormat(Functor functor)
    {
        assert(functor);
        m_functor = std::move(functor);
    }

    //! operator()
    std::string operator()(std::string const& message,
                           Level level,
                           Details const& details) const
    {
        return format(message, level, details);
    }

private:

    Functor m_functor{Default()};

};

/// Default formatter functor
inline std::string
Formatter::Default::operator()(std::string const& msg,
                               Level lvl,
                               Details const& dts) const noexcept
{
    // skip id if its empty
    if (dts.id.empty())
    {
        return gt::log::format("%1 [%2] %3", lvl, dts.time, msg);
    }

    return gt::log::format("%1 [%2] [%3] %4", lvl, dts.time, dts.id, msg);
}

} // end namespace log

} // end namespace gt

#endif // FORMATTER_H
