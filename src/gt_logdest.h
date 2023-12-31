// SPDX-FileCopyrightText: 2013, Razvan Petru
// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef GT_LOGDEST_H
#define GT_LOGDEST_H

#include "gt_logformatter.h"
#include "gt_loglevel.h"

#include <memory>

namespace gt
{
namespace log
{

//! Base class for a logging destination
class Destination
{
public:

    virtual ~Destination() = default;

    //! Default write method to implement. Use a formatter to more easily
    //! format the output message.
    virtual void write(std::string const& message, Level level, Details const& details) = 0;

    //! Returns whether the destination was created correctly
    virtual bool isValid() const { return true; }
};

//! Abstract class for a formatted logging destination
class FormattedDestination : public Destination
{
public:

    //! Will filter and format the message accordingly
    void write(std::string const& message, Level level, Details const& details) final
    {
        // check if level should be logged
        if (filter(level))
        {
            // format message
            write(m_formatter.format(message, level, details), level);
        }
    }

    //! Getter for the current formatter
    Formatter& formatter() { return m_formatter; }
    Formatter const& formatter() const { return m_formatter; }

    //! Setter for the current formatter
    void setFormatter(Formatter formatter)
    {
        m_formatter = std::move(formatter);
    }

    //! Returns whether this level should be logged
    bool filter(Level level)
    {
        return m_filter & levelToInt(level);
    }

    //! Sets the filter level
    FormattedDestination& filterLevel(Level level, bool include = true)
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
    FormattedDestination& filterAll(bool include = true)
    {
        m_filter &= include ? -1 : 0;
        return *this;
    }

protected:

    //! ctor
    FormattedDestination(Formatter formatter = {})
        : m_formatter{std::move(formatter)}
    { }

    //! write method to implement by sub classes
    virtual void write(std::string const& message, Level level) = 0;

private:

    /// formatter
    Formatter m_formatter;

    /// Bitfield to filter out only certain levels (by default all levels)
    int m_filter{-1};
};

using DestinationPtr = std::unique_ptr<Destination>;

} // end namespace log

} // end namespace gt

#endif // GT_LOGDEST_H
