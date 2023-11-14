// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

// log without space
#define GT_LOG_NOSPACE

#include "test_log_helper.h"
#include "gt_logging/qt_bindings.h"

class LogNoSpace : public LogHelperTest
{};

TEST_F(LogNoSpace, logQString)
{
    gtWarning() << QString{"Hello"} << QString{"World"};
    EXPECT_TRUE(log.contains("HelloWorld"));
}

TEST_F(LogNoSpace, logQChar)
{
    gtWarning() << 'T' << QChar{'E'} << 'S' << "T";
    EXPECT_TRUE(log.contains("TEST"));
}

// member methods should be callable as well
TEST_F(LogNoSpace, callMemberMethod)
{
    gtWarning().space() << 'T' << QChar{'E'} << 'S' << "T";
    EXPECT_TRUE(log.contains("T E S T"));
}
