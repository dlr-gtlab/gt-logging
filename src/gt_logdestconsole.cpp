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

#include "gt_logdestconsole.h"
#include "gt_logging.h"

using namespace gt;

namespace
{

void output(std::string const& message, log::Level level);

} // namespace

#if !defined(_WIN32) || (defined(_WIN32) && defined(GT_LOG_WIN_PRINTF_CONSOLE))

#include <cstdio>

namespace
{

void output(std::string const& message, log::Level level)
{
    auto stream = level >= gt::log::WarnLevel ? stderr : stdout;

    fprintf(stream, "%s\n", message.c_str());
    fflush(stream);
}

} // namespace

#else

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace
{

void output(std::string const& message, log::Level)
{
   OutputDebugStringA(message.c_str());
   OutputDebugStringA("\n");
}

} // namespace

#endif

void
log::DebugOutputDestination::write(std::string const& message, Level level)
{
    output(log::Logger::levelToString(level) + " " + message, level);
}
