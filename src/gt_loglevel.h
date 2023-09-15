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

#ifndef GT_LOGLEVEL_H
#define GT_LOGLEVEL_H

#include "gt_logging_exports.h"

#include <string>
#include <ctime>

namespace gt
{

namespace log
{

//! Log levels
enum Level : int
{
    TraceLevel   =  1,
    DebugLevel   =  2,
    InfoLevel    =  4,
    WarnLevel [[deprecated("use WarningLevel instead")]] =  8,
    WarningLevel = 8,
    ErrorLevel   = 16,
    FatalLevel   = 32,
    OffLevel     =  0
};

//! Enum for verbosity log levels
enum Verbosity
{
    Silent = 0,
    Medium = 5,
    Everything = 9
};

//! Struct for message details
struct Details
{
    std::string id;
    std::tm time;
};

GT_LOGGING_EXPORT
Level levelFromString(std::string const& logMessage, bool* ok = {});

GT_LOGGING_EXPORT
std::string levelToString(Level level);

inline Level levelFromInt(int level)
{
    return static_cast<Level>(level);
}

inline int levelToInt(Level level)
{
    return static_cast<int>(level);
}

} // namespace log

} // namespace gt

#endif // GT_LOGLEVEL_H
