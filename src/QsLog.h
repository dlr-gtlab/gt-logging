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

#ifndef QSLOG_H
#define QSLOG_H

#include "QsLogLevel.h"
#include "QsLogDest.h"
#include <QDebug>
#include <QString>
#include <qglobal.h>

#include <gt_logstream.h>

#include <iostream>

// macros to convert an argument to string
#define GT_LOG_TO_STR_HELPER(X) #X
#define GT_LOG_TO_STR(X) GT_LOG_TO_STR_HELPER(X)

// logging id
#ifndef GT_MODULE_ID
#define GT_MODULE_ID ""
#endif

namespace QsLogging
{
class Destination;
class LoggerImpl; // d pointer

class QSLOG_SHARED_OBJECT Logger
{
public:
    static Logger& instance();
    static Level levelFromLogMessage(const QString& logMessage, bool* conversionSucceeded = 0);
    static Level levelFromInt(const int lvl);
    static QString levelToString(Level level);

    ~Logger();

    //! Adds a log message destination. Don't add null destinations.
    void addDestination(DestinationPtr destination);
    //! Removes a previously added destination. Does nothing if destination was not previously added.
    void removeDestination(const DestinationPtr& destination);
    //! Checks if a destination of a specific type has been added. Pass T::Type as parameter.
    bool hasDestinationOfType(const char* type) const;
    //! Logging at a level < 'newLevel' will be ignored
    void setLoggingLevel(Level newLevel);
    //! The default level is INFO
    Level loggingLevel() const;

    //! Sets the verbosity level of the logger (from 0 ... 9)
    void setVerbosity(int);

    //! Gets the verbosity level of the logger (from 0 ... 9)
    int verbosity() const;

    //! The helper forwards the streaming to QDebug and builds the final
    //! log message.
    class QSLOG_SHARED_OBJECT Helper
    {
    public:
        explicit Helper(Level logLevel, QString logId = GT_MODULE_ID);
        ~Helper();

        gt::log::Stream& stream()
        {
            return qtDebug
#ifndef GT_LOG_USE_QUOTE
                .noquote()
#endif
#ifdef GT_LOG_USE_NOSPACE
                .nospace()
#endif
            ;
        }

    private:
        void writeToLog();

        Level level;
        QString buffer;
        gt::log::Stream qtDebug;
        QString id;
    };

private:
    Logger();
    Logger(const Logger&);            // not available
    Logger& operator=(const Logger&); // not available

    void enqueueWrite(const QString& message, Level level);
    void write(const QString& message, Level level);

    LoggerImpl* d;

    friend class LogWriterRunnable;
};

} // end namespace


//lLogging line numbers
#ifdef GT_LOG_LINE_NUMBERS
  // add extra space when logging line numbers
  #ifdef GT_LOG_USE_NOSPACE
    #define GT_LOG_IMPL_SPACE " "
  #else
    #define GT_LOG_IMPL_SPACE
  #endif  
  // call operator explicitly to return stream object
  #define GT_LOG_IMPL_LINE_NUMBERS .operator<<( __FILE__ "@" GT_LOG_TO_STR(__LINE__) ":" GT_LOG_IMPL_SPACE)
#else
  #define GT_LOG_IMPL_LINE_NUMBERS
#endif

//! Logging macros: define GT_LOG_LINE_NUMBERS to get the file and line number
//! in the log output.
#define gtTrace() \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::TraceLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::TraceLevel).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtDebug() \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::DebugLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::DebugLevel).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtInfo()  \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::InfoLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::InfoLevel).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtWarning()  \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::WarnLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::WarnLevel).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtError() \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::ErrorLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::ErrorLevel).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtFatal() \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::FatalLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::FatalLevel).stream() GT_LOG_IMPL_LINE_NUMBERS

//! Logging macros with fixed logging id
#define gtTraceId(ID) \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::TraceLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::TraceLevel, ID).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtDebugId(ID) \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::DebugLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::DebugLevel, ID).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtInfoId(ID)  \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::InfoLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::InfoLevel, ID).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtWarningId(ID)  \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::WarnLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::WarnLevel, ID).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtErrorId(ID) \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::ErrorLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::ErrorLevel, ID).stream() GT_LOG_IMPL_LINE_NUMBERS
#define gtFatalId(ID) \
    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::FatalLevel) {} \
    else QsLogging::Logger::Helper(QsLogging::FatalLevel, ID).stream() GT_LOG_IMPL_LINE_NUMBERS

#ifdef QS_LOG_DISABLE
#include "QsLogDisableForThisFile.h"
#endif

#endif // QSLOG_H
