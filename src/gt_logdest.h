// Copyright (c) 2013, Razvan Petru
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
#include <string>
#include <chrono>

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
    virtual void write(Level /*level*/,
                       std::string const& /*id*/,
                       std::string const& /*message*/,
                       std::tm const& /*time*/) {};

    //! Write method for logging simply informative text to the user
    //! (i.e. best without any formatting)
    virtual void writeInformative(std::string const& /*message*/) {};

    //! Returns whether the destination was created correctly
    virtual bool isValid() const { return true; }

    //! Returns the type as a string e.g: console, file.
    //! The returned value may change in different versions of GtLog,
    //! but two destinations of the same type will return the same value.
    virtual std::string type() const = 0;
};

//! Abstract class for a formatted logging destination
class FormattedDestination : public Destination
{
public:

    //! Will filter and format the message accordingly
    void write(Level level,
               std::string const& id,
               std::string const& message,
               std::tm const& time) final
    {
        // check if level should be logged
        if (m_formatter.filter(level))
        {
            // format message
            write(m_formatter.format(level, id, message, time), level);
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
};

using DestinationPtr = std::shared_ptr<Destination>;

} // end namespace log

} // end namespace gt

#endif // GT_LOGDEST_H
