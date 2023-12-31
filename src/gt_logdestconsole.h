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

#ifndef GT_LOGDESTCONSOLE_H
#define GT_LOGDESTCONSOLE_H

#include "gt_logdest.h"
#include "gt_logformatter.h"

namespace gt
{

namespace log
{

// debugger sink
class DebugOutputDestination : public FormattedDestination
{
public:

    //! ctor
    explicit DebugOutputDestination(Formatter formatter = {})
        : FormattedDestination{std::move(formatter)}
    { }

    //! Logs formatted text
    void write(std::string const& message, Level level) override
    {
        output(message, level);
    }

private:

    //! Helper method for logging to console
    GT_LOGGING_EXPORT
    void output(std::string const& message, Level level);
};

inline std::unique_ptr<DebugOutputDestination>
makeDebugOutputDestination(Formatter formatter = {})
{
    return std::make_unique<DebugOutputDestination>(std::move(formatter));
}


} // namespace log

} // namespace gt

#endif // GT_LOGDESTCONSOLE_H
