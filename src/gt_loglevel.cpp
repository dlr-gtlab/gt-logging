/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#include "gt_loglevel.h"

#include <cassert>
using namespace gt;

static const char TraceString[] = "TRACE";
static const char DebugString[] = "DEBUG";
static const char InfoString[]  = "INFO ";
static const char WarnString[]  = "WARN ";
static const char ErrorString[] = "ERROR";
static const char FatalString[] = "FATAL";

static const char* LevelToText(log::Level theLevel)
{
    switch (theLevel) {
        case log::TraceLevel:
            return TraceString;
        case log::DebugLevel:
            return DebugString;
        case log::InfoLevel:
            return InfoString;
        case log::WarnLevel:
            return WarnString;
        case log::ErrorLevel:
            return ErrorString;
        case log::FatalLevel:
            return FatalString;
        case log::OffLevel:
            return "";
        default: {
            assert(!"bad log level");
            return InfoString;
        }
    }
}

// tries to extract the level from a string log message. If available, conversionSucceeded will
// contain the conversion result.
log::Level
log::levelFromString(std::string const& logMessage, bool* ok)
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



std::string
log::levelToString(Level level)
{
    return LevelToText(level);
}
