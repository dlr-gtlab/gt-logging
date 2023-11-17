// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#include "gt_logging.h"
#include <gtest/gtest.h>

struct LogLevel : public testing::Test
{
    void testLevel(gt::log::Level level)
    {
        using namespace gt::log;
        // print current iteration
        static int i = 0;
        std::cout << i++ << ": Level: " << level << '\n';

        auto levelStr = levelToString(level);
        EXPECT_FALSE(levelStr.empty());
        EXPECT_EQ(levelFromString(levelStr), level);
    }

};

// member methods should be callable as well
TEST_F(LogLevel, toString)
{
    using namespace gt::log;
    testLevel(TraceLevel);
    testLevel(DebugLevel);
    testLevel(InfoLevel);
    testLevel(WarningLevel);
    testLevel(ErrorLevel);
    testLevel(FatalLevel);
}
