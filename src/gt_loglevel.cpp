// SPDX-FileCopyrightText: 2013, Razvan Petru
// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#include "gt_loglevel.h"

#include <cassert>
using namespace gt;

static constexpr char TraceString[] = "TRACE";
static constexpr char DebugString[] = "DEBUG";
static constexpr char InfoString[]  = "INFO ";
static constexpr char WarnString[]  = "WARN ";
static constexpr char ErrorString[] = "ERROR";
static constexpr char FatalString[] = "FATAL";

inline const char* LevelToText(log::Level level)
{
    switch (level) {
        case log::TraceLevel:
            return TraceString;
        case log::DebugLevel:
            return DebugString;
        case log::InfoLevel:
            return InfoString;
        case log::WarningLevel:
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
        return WarningLevel;
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
