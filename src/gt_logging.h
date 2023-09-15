// Based on QsLog 2.1

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

#ifndef GTLOGGING_H
#define GTLOGGING_H

#include "gt_loglevel.h"
#include "gt_logstream.h"

#include <vector>

#include "gt_logdestconsole.h"
#include "gt_logdestfile.h"
#include "gt_logdestfunctor.h"

// macros to convert an argument to string
#define GT_LOG_IMPL_TO_STR(X) #X
#define GT_LOG_TO_STR(X) GT_LOG_IMPL_TO_STR(X)

// logging id
#ifndef GT_MODULE_ID
#define GT_MODULE_ID ""
#endif

namespace gt
{

namespace log
{

using hash_t = size_t;

GT_LOGGING_EXPORT
hash_t hash(std::string const& msg, std::string const& id, Level level);

//! Main logger instance
class Logger
{
public:

    //! Default cache to store the hashes of the messages that should be
    //! logged once. This cache is thread safe, thus may not have optimal
    //! performance. Look up time is O(n).
    class DefaultCache
    {
    public:
        using value_type = hash_t;

        GT_LOGGING_EXPORT DefaultCache();
        GT_LOGGING_EXPORT ~DefaultCache();

        //! Appends a new hash entry to the cache.
        GT_LOGGING_EXPORT void append(hash_t hash);

        //! Clears all saved hashes
        GT_LOGGING_EXPORT void clear();

        //! Returns whether the hash exists in the cache
        GT_LOGGING_EXPORT bool find(hash_t hash) const noexcept;

    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
    };

    DefaultCache globalCache;

    GT_LOGGING_EXPORT
    static Logger& instance();

    GT_LOGGING_EXPORT
    ~Logger();

    //! Adds a named log message destination if its valid. Dont use use
    //! empty ids. Will skip duplicates (by name).
    GT_LOGGING_EXPORT
    bool addDestination(std::string id, DestinationPtr destination);

    //! Removes a previously added destination by name.Returns true if a
    //! destination was removed.
    GT_LOGGING_EXPORT
    bool removeDestination(const std::string& id);

    //! Checks if the named destination has been added.
    GT_LOGGING_EXPORT
    bool hasDestination(const std::string& id);

    //! Returns the destination specified by name. Returns null if the
    //! destion was not found.
    GT_LOGGING_EXPORT
    Destination* destination(const std::string& id) const;

    //! Returns a list of all registered destination ids
    GT_LOGGING_EXPORT
    std::vector<std::string> destinationIds() const;

    //! Logging at a level < 'newLevel' will be ignored
    GT_LOGGING_EXPORT
    void setLoggingLevel(Level newLevel);

    //! The default level is INFO
    GT_LOGGING_EXPORT
    Level loggingLevel() const;

    //! Sets the verbosity level of the logger (from 0 ... 9)
    GT_LOGGING_EXPORT
    void setVerbosity(int verbosity);

    //! Gets the verbosity level of the logger (from 0 ... 9)
    GT_LOGGING_EXPORT
    int verbosity() const;

    GT_LOGGING_EXPORT
    void log(Level level, std::string id, std::string message);

    //! The helper forwards the streaming to QDebug and builds the final
    //! log message.
    class Helper
    {
    public:

        explicit Helper(Level _level, std::string _id = GT_MODULE_ID) :
            level{_level},
            id{std::move(_id)}
        {}

        ~Helper() { writeToLog(); }

        gt::log::Stream& stream() { return gtStream; }
    private:

        Level level;
        std::string id;
        Stream gtStream;

        GT_LOGGING_EXPORT void writeToLog();
    };

private:

    Logger();
    Logger(Logger const&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger const&) = delete;
    Logger& operator=(Logger&&) = delete;

    void write(std::string const& message, Level level, Details details);

    struct Impl; // d pointer
    std::unique_ptr<Impl> pimpl;
};

template <typename Cache>
class LogOnce
{
public:

    explicit LogOnce(Cache& _cache, Level _level, std::string _id = GT_MODULE_ID) :
        level{_level},
        id{std::move(_id)},
        cache(&_cache)
    {}

    LogOnce(LogOnce&&) = default;
    ~LogOnce()
    {
        if (!gtStream.mayLog()) return;

        std::string message = gtStream.str();
        if (message.empty()) return;

        hash_t hash = gt::log::hash(message, id, level);

        bool hashExists = cache->find(hash);
        if (hashExists) return;

        cache->append(hash);
        Logger::instance().log(level, std::move(id), std::move(message));
    }

    gt::log::Stream& stream() { return gtStream; }

private:

    Level level;
    std::string id;
    Stream gtStream;
    Cache* cache;
};

//! Constructs a LogOnce Helper object that uses a custom cache
template <typename Cache>
inline auto logOnce(Cache& cache, Level level, std::string id  = GT_MODULE_ID)
{
    return LogOnce<Cache>(cache, level, std::move(id));
}

//! Constructs a LogOnce Helper object that uses the global cache
inline auto logOnce(Level level, std::string id  = GT_MODULE_ID)
{
    return logOnce(Logger::instance().globalCache, level, std::move(id));
}

} // namespace log

} // namespace gt

// log line numbers
#ifdef GT_LOG_LINE_NUMBERS
// call operator explicitly to return stream object
#define GT_LOG_IMPL_LINE_NUMBERS .operator<<( __FILE__ "@" GT_LOG_TO_STR(__LINE__) ":")
#else
#define GT_LOG_IMPL_LINE_NUMBERS
#endif

#ifdef GT_LOG_QUOTE
// enable quoting of string types
#define GT_LOG_IMPL_QUOTE .quote()
#else
#define GT_LOG_IMPL_QUOTE
#endif

#ifdef GT_LOG_NOSPACE
// disable logging of spaces
#define GT_LOG_IMPL_NOSPACE .nospace()
#else
#define GT_LOG_IMPL_NOSPACE
#endif

////////// HELPER MACROS FOR COMON CODE //////////

// log only if logging level matches
#define GT_LOG_IMPL_IF_LEVEL(LEVEL) \
    if (gt::log::Logger::instance().loggingLevel() <= gt::log::LEVEL)

// apply global flags (quote, nospace, line numbers)
#define GT_LOG_IMPL_APPLY_FLAGS() \
    GT_LOG_IMPL_LINE_NUMBERS GT_LOG_IMPL_QUOTE GT_LOG_IMPL_NOSPACE

#define GT_LOG_IMPL_NO_ARG_EXPANDER() ,,

////////// DEFAULT LOGGING MACROS //////////

#define GT_LOG_IMPL_MESSAGE(LEVEL) \
    GT_LOG_IMPL_IF_LEVEL(LEVEL) \
        gt::log::Logger::Helper(gt::log::LEVEL).stream() \
            GT_LOG_IMPL_APPLY_FLAGS()

//! Default logging macros
#define gtTrace()       GT_LOG_IMPL_MESSAGE(TraceLevel)
#define gtDebug()       GT_LOG_IMPL_MESSAGE(DebugLevel)
#define gtInfo()        GT_LOG_IMPL_MESSAGE(InfoLevel)
#define gtWarning()     GT_LOG_IMPL_MESSAGE(WarningLevel)
#define gtError()       GT_LOG_IMPL_MESSAGE(ErrorLevel)
#define gtFatal()       GT_LOG_IMPL_MESSAGE(FatalLevel)

////////// DEFAULT LOGGING MACROS WITH ID //////////

#define GT_LOG_IMPL_MEESAGE_ID(LEVEL, ID) \
    GT_LOG_IMPL_IF_LEVEL(LEVEL) \
        gt::log::Logger::Helper(gt::log::LEVEL, ID).stream() \
            GT_LOG_IMPL_APPLY_FLAGS()

#define gtTraceId(ID)   GT_LOG_IMPL_MEESAGE_ID(TraceLevel, ID)
#define gtDebugId(ID)   GT_LOG_IMPL_MEESAGE_ID(DebugLevel, ID)
#define gtInfoId(ID)    GT_LOG_IMPL_MEESAGE_ID(InfoLevel , ID)
#define gtWarningId(ID) GT_LOG_IMPL_MEESAGE_ID(WarningLevel , ID)
#define gtErrorId(ID)   GT_LOG_IMPL_MEESAGE_ID(ErrorLevel, ID)
#define gtFatalId(ID)   GT_LOG_IMPL_MEESAGE_ID(FatalLevel, ID)

////////// LOG ONCE MACRO //////////

#define GT_LOG_IMPL_ONCE_F1(LEVEL, ...) \
    GT_LOG_IMPL_IF_LEVEL(LEVEL ## Level) \
        gt::log::logOnce(gt::log::LEVEL ## Level).stream() \
            GT_LOG_IMPL_APPLY_FLAGS()
#define GT_LOG_IMPL_ONCE_F2(LEVEL, CACHE, ...) \
    GT_LOG_IMPL_IF_LEVEL(LEVEL ## Level) \
        gt::log::logOnce(CACHE, gt::log::LEVEL ## Level).stream() \
            GT_LOG_IMPL_APPLY_FLAGS()

// variadic macro magic
#define GT_LOG_IMPL_ONCE_FUNC_CHOOSER(_f1, _f2, _f3, ...) _f3
#define GT_LOG_IMPL_ONCE_FUNC_RECOMPOSER(ArgsWithBrackets) GT_LOG_IMPL_ONCE_FUNC_CHOOSER ArgsWithBrackets
#define GT_LOG_IMPL_ONCE_CHOOSE_FROM_ARG_COUNT(...) GT_LOG_IMPL_ONCE_FUNC_RECOMPOSER((__VA_ARGS__, GT_LOG_IMPL_ONCE_F2, GT_LOG_IMPL_ONCE_F1, ))
#define GT_LOG_IMPL_ONCE_MACRO_CHOOSER(...) GT_LOG_IMPL_ONCE_CHOOSE_FROM_ARG_COUNT(GT_LOG_IMPL_NO_ARG_EXPANDER __VA_ARGS__ ())

#define gtLogOnce(...) GT_LOG_IMPL_ONCE_MACRO_CHOOSER( __VA_ARGS__)( __VA_ARGS__)

////////// LOG ONCE WITH ID MACRO //////////

#define GT_LOG_IMPL_ONCE_ID_F2(LEVEL, ID, ...) \
    GT_LOG_IMPL_IF_LEVEL(LEVEL ## Level) \
        gt::log::logOnce(gt::log::LEVEL ## Level, ID).stream() \
            GT_LOG_IMPL_APPLY_FLAGS()
#define GT_LOG_IMPL_ONCE_ID_F3(LEVEL, ID, CACHE, ...) \
    GT_LOG_IMPL_IF_LEVEL(LEVEL ## Level) \
        gt::log::logOnce(CACHE, gt::log::LEVEL ## Level, ID).stream() \
            GT_LOG_IMPL_APPLY_FLAGS()

// variadic macro magic
#define GT_LOG_IMPL_ONCE_ID_FUNC_CHOOSER(_f1, _f2, _f3, _f4, ...) _f4
#define GT_LOG_IMPL_ONCE_ID_FUNC_RECOMPOSER(ArgsWithBrackets) GT_LOG_IMPL_ONCE_ID_FUNC_CHOOSER ArgsWithBrackets
#define GT_LOG_IMPL_ONCE_ID_CHOOSE_FROM_ARG_COUNT(...) GT_LOG_IMPL_ONCE_ID_FUNC_RECOMPOSER((__VA_ARGS__, GT_LOG_IMPL_ONCE_ID_F3, GT_LOG_IMPL_ONCE_ID_F2, ,))
#define GT_LOG_IMPL_ONCE_ID_MACRO_CHOOSER(...) GT_LOG_IMPL_ONCE_ID_CHOOSE_FROM_ARG_COUNT(GT_LOG_IMPL_NO_ARG_EXPANDER __VA_ARGS__ ())

#define gtLogOnceId(...) GT_LOG_IMPL_ONCE_ID_MACRO_CHOOSER( __VA_ARGS__)( __VA_ARGS__)

////////// APPLY GLOBAL DEFINES //////////

#ifdef GT_LOG_DISABLE
#include "gt_logdisablelogforfile.h"
#endif

// Additional stl bindings
#ifdef GT_LOG_USE_EXTENDED_STL_BINDINGS
#include "gt_logging/stl_bindings.h"
#endif

// Qt support
#ifdef GT_LOG_USE_QT_BINDINGS
#include "gt_logging/qt_bindings.h"
#endif

#endif // GTLOGGING_H
