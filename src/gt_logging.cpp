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

#include "gt_logging.h"

#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>
#include <algorithm>

using MutexLocker = const std::lock_guard<std::mutex>;

namespace gt
{
namespace log
{

hash_t hash(std::string const& msg, std::string const& id, Level level)
{
    std::hash<std::string> hasher;
    return hasher(msg + id + std::to_string(level));
}

struct Logger::DefaultCache::Impl
{
    Impl()
    {
        cache.reserve(128);
    }

    std::mutex mutex;
    std::vector<hash_t> cache;
};

Logger::DefaultCache::DefaultCache() : pimpl(std::make_unique<Impl>()) { }

Logger::DefaultCache::~DefaultCache() = default;

void
Logger::DefaultCache::append(hash_t hash)
{
    MutexLocker locker(pimpl->mutex);
    pimpl->cache.push_back(hash);
}

void
Logger::DefaultCache::clear()
{
    MutexLocker locker(pimpl->mutex);
    pimpl->cache.clear();
}

bool
Logger::DefaultCache::find(hash_t hash) const noexcept
{
    MutexLocker locker(pimpl->mutex);
    return std::find(pimpl->cache.begin(), pimpl->cache.end(), hash) != pimpl->cache.end();
}

struct DestinationEntry
{
    std::string id;
    DestinationPtr ptr;
};

struct Logger::Impl
{
    Impl()
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
                  << "' already exists!\n";
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

    MutexLocker lock(pimpl->logMutex);

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
    MutexLocker lock(pimpl->logMutex);

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

std::vector<std::string>
Logger::destinationIds() const
{
    std::vector<std::string> ids;
    ids.reserve(pimpl->destinations.size());
    std::transform(pimpl->destinations.cbegin(), pimpl->destinations.cend(),
                   std::back_inserter(ids), [](DestinationEntry const& dest){
        return dest.id;
    });
    return ids;
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
Logger::log(Level level, std::string message, std::string id)
{
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

    write(message, level, Details{id, *time});
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

void
Logger::Helper::writeToLog()
{
    if (!gtStream.mayLog()) return;

    auto message = gtStream.str();
    if (message.empty()) return;

    Logger::instance().log(level, std::move(message), std::move(id));
}

} // end namespace log

} // end namespace gt

bool
gt::log::Stream::mayLog(int level)
{
    return level <= Logger::instance().verbosity();
}
