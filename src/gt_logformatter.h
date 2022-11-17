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
#include <sstream>

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

    //! Ctor accepting format and filter
    explicit Formatter(std::string format, int filter = -1)
        : m_format(std::move(format))
        , m_filter(filter)
    { }

    //! Setter for the format
    void setFormat(std::string format)
    {
        m_format = std::move(format);
    }

    //! Returns whether this level should be logged
    bool filter(gt::log::Level level)
    {
        return m_filter & levelToInt(level);
    }

    //! Sets the filter level
    void filterLevel(gt::log::Level level, bool include = true)
    {
        m_filter &= include ? levelToInt(level) : ~levelToInt(level);
    }

    //! Default format
    static std::string const& defaultFormat()
    {
        static const std::string format = "/L [/I] [/T] /M";
        return format;
    }

    //! Formats the arguments into a single string acording to the default format.
    static std::string formatDefault(gt::log::Level level,
                                     std::string const& id,
                                     std::string const& message,
                                     std::tm time)
    {
        return format(defaultFormat(), level, id, message, time);
    }

    //! Formats the arguments into a single string acording to the format.
    static std::string format(std::string const& format,
                              gt::log::Level level,
                              std::string const& id,
                              std::string const& message,
                              std::tm time)
    {
        // unformatted
        if (level == InfoLevel && time.tm_mday == 0 && time.tm_sec == 0)
        {
            return message;
        }

        std::string formatted = format;
        // size +10 for good measure
        formatted.reserve(format.size() + message.size() + id.size() + 10);

        formatTime(formatted, time);
        replace("/L", formatted, levelToString(level));
        replace("/I", formatted, id.empty() ? "-" : id);
        replace("/M", formatted, message);

        return formatted;
    }

    //! Formats the arguments into a single string acording to the format.
    std::string format(gt::log::Level level,
                       std::string const& id,
                       std::string const& message,
                       std::tm time) const
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
     * L = Level
     * I = Id
     * M = Message
     * T = Day time (%H:%M:%S)
     */
    std::string m_format{defaultFormat()};

    int m_filter{-1};

    //! Replaces ident if it exists
    static void replace(std::string ident,
                        std::string& string,
                        std::string const& r)
    {
        size_t idx = string.find(ident);
        if (idx != std::string::npos)
        {
            string.replace(idx, ident.size(), r);
        }
    }

    //! Replaces time ident if it exists with default time format
    static void formatTime(std::string& string, tm& time)
    {
        size_t idx = string.find("/T");
        if (idx != std::string::npos)
        {
            // time format
            static constexpr char timeFormat[] = "%H:%M:%S";
            // time buffer
            std::array<char, sizeof(timeFormat)> timestr;

            std::strftime(timestr.data(), timestr.size(), timeFormat, &time);
            string.replace(idx, 2, timestr.data());
        }
    }
};

} // end namespace log

} // end namespace gt

#endif // FORMATTER_H
