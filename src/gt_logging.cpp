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

#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>
#include <algorithm>

namespace gt
{
namespace log
{

using MutexLocker = const std::lock_guard<std::mutex>;

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

Logger::~Logger() = default;

bool
Logger::addDestination(std::string id, DestinationPtr destination)
{
    if (!destination || !destination->isValid())
    {
        std::cerr << "GtLogging: Invalid destination!\n";
        return false;
    }

    if (id.empty())
    {
        std::cerr << "GtLogging: Invalid destination id!\n";
        return false;
    }

    // mutex is already locked here
    if (hasDestination(id))
    {
        std::cerr << "GtLogging: A destination named '" << id
                  << "' already exsits!\n";
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

} // namespace

bool
Logger::removeDestination(const std::string& id)
{
    if (id.empty()) return false;

    return erase(pimpl->destinations, [&](DestinationEntry const& dest){
        return dest.id == id;
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

Destination*
Logger::destination(const std::string& id) const
{
    auto iter = std::find_if(pimpl->destinations.begin(),
                             pimpl->destinations.end(),
                             [&](DestinationEntry const& dest){
        return dest.id == id;
    });

    if (iter != std::cend(pimpl->destinations))
    {
        return iter->ptr.get();
    }
    return {};
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

void
Logger::Helper::writeToLog()
{
    auto message = gtStream.str();
    if (message.empty())
    {
        return;
    }

    // get time
    std::tm timebuf;
    std::time_t rawtime;
    std::time(&rawtime);

    // https://en.cppreference.com/w/c/chrono/localtime
#ifdef _WIN32
    std::tm* time = &timebuf;
    localtime_s(&timebuf, &rawtime);
#else
    tm* time = localtime_r(&rawtime, &timebuf);
#endif

    Logger::instance().write(message, level, Details{id, *time});
}

//! Sends the message to all the destinations. The level for this message is passed in case
//! it's useful for processing in the destination.
void
Logger::write(std::string const& message, Level level, Details details)
{
    MutexLocker lock(pimpl->logMutex);

    std::for_each(pimpl->destinations.begin(), pimpl->destinations.end(),
                  [&](DestinationEntry const& dest){
        dest.ptr->write(message, level, details);
    });
}

} // end namespace log

} // end namespace gt

bool
gt::log::Stream::mayLog(int level)
{
    return level <= Logger::instance().verbosity();
}