// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause


#include "test_log_helper.h"

struct LogLevel : public LogHelperTest
{
    static void testLevel(gt::log::Level level)
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

// member methods should be callable as well
TEST_F(LogLevel, offLevel)
{
    using namespace gt::log;

    Logger::instance().setLoggingLevel(gt::log::DebugLevel);

    gtDebug() << "Test";
    EXPECT_TRUE(log.contains("Test"));

    gtInfo() << "Hello";
    EXPECT_TRUE(log.contains("Hello"));

    gtTrace() << "Trace";
    EXPECT_FALSE(log.contains("Trace"));

    log.clear();

    // turn logging off
    Logger::instance().setLoggingLevel(gt::log::OffLevel);

    gtDebug() << "Test";
    EXPECT_FALSE(log.contains("Test"));

    gtInfo() << "Hello";
    EXPECT_FALSE(log.contains("Hello"));

    gtFatal() << "Fatal";
    EXPECT_FALSE(log.contains("Fatal"));
}
