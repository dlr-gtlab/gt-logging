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

static constexpr char TraceString[] = "TRACE";
static constexpr char DebugString[] = "DEBUG";
static constexpr char InfoString[]  = "INFO ";
static constexpr char WarnString[]  = "WARN ";
static constexpr char ErrorString[] = "ERROR";
static constexpr char FatalString[] = "FATAL";

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

    if (logMessage.find(TraceString) != std::string::npos)
        return TraceLevel;
    if (logMessage.find(DebugString) != std::string::npos)
        return DebugLevel;
    if (logMessage.find(InfoString) != std::string::npos)
        return InfoLevel;
    if (logMessage.find(WarnString) != std::string::npos)
        return WarnLevel;
    if (logMessage.find(ErrorString) != std::string::npos)
        return ErrorLevel;
    if (logMessage.find(FatalString) != std::string::npos)
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
