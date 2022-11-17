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

#include "gt_logging.h"

#include <chrono>
#include <mutex>
#include <array>
#include <vector>

namespace gt
{
namespace log
{

static const char TraceString[] = "TRACE";
static const char DebugString[] = "DEBUG";
static const char InfoString[]  = "INFO ";
static const char WarnString[]  = "WARN ";
static const char ErrorString[] = "ERROR";
static const char FatalString[] = "FATAL";

static const size_t DEFAULT_BUFFER_SIZE = 60;

using MutexLocker = const std::lock_guard<std::mutex>;

static const char* LevelToText(Level theLevel)
{
    switch (theLevel) {
        case TraceLevel:
            return TraceString;
        case DebugLevel:
            return DebugString;
        case InfoLevel:
            return InfoString;
        case WarnLevel:
            return WarnString;
        case ErrorLevel:
            return ErrorString;
        case FatalLevel:
            return FatalString;
        case OffLevel:
            return "";
        default: {
            assert(!"bad log level");
            return InfoString;
        }
    }
}

struct DestinationEntry
{
    std::string id;
    DestinationPtr ptr;
};

struct Logger::Impl
{
    Impl() : level()
    {
        // assume at least file + console
        destinations.reserve(2);
    }

    Level level{InfoLevel};
    int verbosity{Silent};
    std::mutex logMutex;
    std::vector<DestinationEntry> destinations;
};

Logger::Logger() : pimpl(std::make_unique<Impl>()) { }

Logger&
Logger::instance()
{
    static Logger instance;
    return instance;
}

// tries to extract the level from a string log message. If available, conversionSucceeded will
// contain the conversion result.
Level
Logger::levelFromString(const std::string& logMessage, bool* ok)
{
    if (ok)
    {
        *ok = true;
    }

    if (logMessage == TraceString)
        return TraceLevel;
    if (logMessage == DebugString)
        return DebugLevel;
    if (logMessage == InfoString)
        return InfoLevel;
    if (logMessage == WarnString)
        return WarnLevel;
    if (logMessage == ErrorString)
        return ErrorLevel;
    if (logMessage == FatalString)
        return FatalLevel;

    if (ok)
    {
        *ok = false;
    }

    return OffLevel;
}

Level
Logger::levelFromInt(const int lvl)
{
    return static_cast<Level>(lvl);
}

std::string
Logger::levelToString(Level level)
{
    return LevelToText(level);
}

Logger::~Logger() = default;

bool
Logger::addDestination(DestinationPtr destination)
{
    if (!destination || !destination->isValid())
    {
        std::cerr << "GtLog: Invalid destination!\n";
        return false;
    }

    MutexLocker lock(pimpl->logMutex);
    pimpl->destinations.push_back({ std::string{}, std::move(destination) });
    return true;
}

bool
Logger::addDestination(std::string id, DestinationPtr destination)
{
    if (!destination || !destination->isValid())
    {
        std::cerr << "GtLog: Invalid destination!\n";
        return false;
    }

    if (id.empty())
    {
        std::cerr << "GtLog: Invalid destination id!\n";
        return false;
    }

    // mutex is already locked here
    if (hasDestination(id))
    {
        std::cerr << "GtLog: The destination named '" << id
                  << "' does already exsits and wont be added!\n";
        return false;
    }

    MutexLocker lock(pimpl->logMutex);
    pimpl->destinations.push_back({ std::move(id), std::move(destination) });
    return true;
}

namespace
{

template <typename Op>
inline auto
erase(std::vector<DestinationEntry>& destinations, Op op)
{
    auto iter = std::remove_if(destinations.begin(),
                               destinations.end(),
                               op);

    // nothing found
    if (iter == destinations.end()) return false;

    // erase entry
    destinations.erase(iter, destinations.end());
    return true;
}

template <typename Op>
inline bool
any_of(std::vector<DestinationEntry> const& destinations, Op op)
{
    return std::any_of(destinations.begin(), destinations.end(), op);
}

template <size_t N>
inline void
setCurrentTime(std::array<char, N>& buffer, const char* format)
{
    // get time
    struct tm timebuf;
    auto timep = std::chrono::system_clock::now();
    auto time  = std::chrono::system_clock::to_time_t(timep);

    // https://en.cppreference.com/w/c/chrono/localtime
#ifdef _WIN32
    tm* t = &timebuf;
    localtime_s(&timebuf, &time);
#else
    tm* t = localtime_r(&time, &timebuf);
#endif

    strftime(buffer.data(), N, format, t);
}

} // namespace

bool
Logger::removeDestination(const DestinationPtr& destination)
{
    MutexLocker lock(pimpl->logMutex);

    return erase(pimpl->destinations, [&](DestinationEntry const& dest){
        return dest.ptr.get() == destination.get();
    });
}

bool
Logger::removeDestination(const std::string& id)
{
    if (id.empty()) return false;

    return erase(pimpl->destinations, [&](DestinationEntry const& dest){
        return dest.id == id;
    });
}

bool
Logger::hasDestinationOfType(std::string const& type) const
{
    MutexLocker lock(pimpl->logMutex);

    return any_of(pimpl->destinations, [&](DestinationEntry const& dest){
        return dest.ptr->type() == type;
    });
}

bool
Logger::hasDestination(const DestinationPtr& destination)
{
    MutexLocker lock(pimpl->logMutex);

    return any_of(pimpl->destinations, [&](DestinationEntry const& dest){
        return dest.ptr.get() == destination.get();
    });
}

bool
Logger::hasDestination(const std::string& id)
{
    MutexLocker lock(pimpl->logMutex);

    return any_of(pimpl->destinations, [&](DestinationEntry const& dest){
        return dest.id == id;
    });
}

void
Logger::setLoggingLevel(Level newLevel)
{
    pimpl->level = newLevel;
}

Level Logger::loggingLevel() const
{
    return pimpl->level;
}

void
Logger::setVerbosity(int verbosity)
{
    pimpl->verbosity = std::move(verbosity);
}

int
Logger::verbosity() const
{
    return pimpl->verbosity;
}

//! creates the complete log message and passes it to the logger
void
Logger::Helper::writeToLog()
{
    auto buffer = gtStream.str();
    if (buffer.empty())
    {
        return;
    }

    // string buffer
    std::string streambuf;
    streambuf.reserve(DEFAULT_BUFFER_SIZE);
    std::ostringstream ostream(streambuf);

    // make id string
    if (!id.empty())
    {
        ostream << '[' << id << "] ";
    }

    // time format
    static constexpr char timeFormat[] = "[%H:%M:%S] ";
    // time buffer
    std::array<char, sizeof(timeFormat)> time;

    // convert time to string
    setCurrentTime(time, timeFormat);

    ostream << time.data();
    ostream << std::move(buffer);

    // write
    Logger::instance().write(ostream.str(), level);
}

Logger::Helper::Helper(Level logLevel, std::string logId) :
    level{logLevel},
    id{std::move(logId)}
{}

Logger::Helper::~Helper()
{
    writeToLog();
}

//! Sends the message to all the destinations. The level for this message is passed in case
//! it's useful for processing in the destination.
void
Logger::write(std::string const& message, Level level)
{
    MutexLocker lock(pimpl->logMutex);

    std::for_each(pimpl->destinations.begin(), pimpl->destinations.end(),
                  [&message, level](DestinationEntry const& dest){
        dest.ptr->write(message, level);
    });
}

} // end namespace log

} // end namespace gt

bool
gt::log::Stream::mayLog(int level)
{
    return level <= Logger::instance().verbosity();
}
