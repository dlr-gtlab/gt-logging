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

#include <array>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace gt
{
namespace log
{

//! Helper class for formatting a log message
class Formatter
{
public:

    //! Default ctor
    Formatter() = default;

    //! ctor accepting a custom format and level filter
    explicit Formatter(std::string format, int filter = -1)
        : m_format(std::move(format))
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
            m_filter |=  levelToInt(level);
        else
            m_filter &= ~levelToInt(level);
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
        static const std::string dformat = "/L [/I] [%H:%M:%S] /M";
        return dformat;
    }

    //! Formats the arguments into a single string acording to the default format.
    static std::string formatDefault(gt::log::Level level,
                                     std::string const& id,
                                     std::string const& message,
                                     std::tm const& time)
    {
        return format(defaultFormat(), level, id, message, time);
    }

    //! Formats the arguments into a single string acording to the format.
    static std::string format(std::string const& format,
                              gt::log::Level level,
                              std::string const& id,
                              std::string const& message,
                              std::tm const& time)
    {
        // format time
        std::ostringstream s;
        s << std::put_time(&time, format.c_str());

        // make formatted string
        std::string formatted{s.str()};
        formatted.reserve(formatted.size() + message.size() + id.size());

        // replace idents
        replace(formatted, "/L", levelToString(level));
        replace(formatted, "/I", id.empty() ? "-" : id);
        replace(formatted, "/M", message);

        return formatted;
    }

    //! Formats the arguments into a single string acording to the format.
    std::string format(gt::log::Level level,
                       std::string const& id,
                       std::string const& message,
                       std::tm const& time) const
    {
        return format(m_format, level, id, message, time);
    }

    //! Getter for format string
    std::string const& format() const
    {
        return m_format;
    }


private:

    /**
     * /L = Level
     * /I = Id
     * /M = Message
     *  % = Time format of std::put_time (e.g. %H:%M:%S)
     */
    std::string m_format{defaultFormat()};

    /// Bitfield to filter out only certain levels (by default all levels)
    int m_filter{-1};

    //! Replaces ident if it exists
    static void replace(std::string& string,
                        std::string const& ident,
                        std::string const& r)
    {
        size_t off = 0;
        size_t idx = string.find(ident, off);

        // loop over all occurrences
        while (idx != std::string::npos)
        {
            // replace ident
            string.replace(idx, ident.size(), r);
            off = idx + 1;
            idx = string.find(ident, off);
        }
    }
};

} // end namespace log

} // end namespace gt

#endif // FORMATTER_H
