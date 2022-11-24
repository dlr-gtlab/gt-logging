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

#include "gt_logdestconsole.h"
#include "gt_logdestfile.h"
#include "gt_logdestfunctor.h"

// macros to convert an argument to string
#define GT_LOG_TO_STR_HELPER(X) #X
#define GT_LOG_TO_STR(X) GT_LOG_TO_STR_HELPER(X)

// logging id
#ifndef GT_MODULE_ID
#define GT_MODULE_ID ""
#endif

namespace gt
{

namespace log
{

class Logger
{
public:
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

    //! Checks if the named destination has been added.
    GT_LOGGING_EXPORT
    Destination* destination(const std::string& id) const;

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

    //! The helper forwards the streaming to QDebug and builds the final
    //! log message.
    class Helper
    {
    public:

        explicit Helper(Level logLevel, std::string logId = GT_MODULE_ID) :
            level{logLevel},
            id{std::move(logId)}
        {}

        ~Helper() { writeToLog(); }

        gt::log::Stream& stream() { return gtStream; }

    private:

        Level level;
        std::string id;
        gt::log::Stream gtStream;

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

#define GT_LOG_IMPL(LEVEL) \
    if (gt::log::Logger::instance().loggingLevel() > gt::log::LEVEL) \
    {} \
    else gt::log::Logger::Helper(gt::log::LEVEL).stream() \
    GT_LOG_IMPL_LINE_NUMBERS GT_LOG_IMPL_QUOTE GT_LOG_IMPL_NOSPACE

#define GT_LOG_IMPL_ID(LEVEL, ID) \
    if (gt::log::Logger::instance().loggingLevel() > gt::log::LEVEL) \
    {} \
    else gt::log::Logger::Helper(gt::log::LEVEL, ID).stream() \
    GT_LOG_IMPL_LINE_NUMBERS GT_LOG_IMPL_QUOTE GT_LOG_IMPL_NOSPACE


//! Default logging macros.
#define gtTrace()       GT_LOG_IMPL(TraceLevel)
#define gtDebug()       GT_LOG_IMPL(DebugLevel)
#define gtInfo()        GT_LOG_IMPL(InfoLevel)
#define gtWarning()     GT_LOG_IMPL(WarnLevel)
#define gtError()       GT_LOG_IMPL(ErrorLevel)
#define gtFatal()       GT_LOG_IMPL(FatalLevel)

//! Logging macros with custom logging id
#define gtTraceId(ID)   GT_LOG_IMPL_ID(TraceLevel, ID)
#define gtDebugId(ID)   GT_LOG_IMPL_ID(DebugLevel, ID)
#define gtInfoId(ID)    GT_LOG_IMPL_ID(InfoLevel , ID)
#define gtWarningId(ID) GT_LOG_IMPL_ID(WarnLevel , ID)
#define gtErrorId(ID)   GT_LOG_IMPL_ID(ErrorLevel, ID)
#define gtFatalId(ID)   GT_LOG_IMPL_ID(FatalLevel, ID)

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
