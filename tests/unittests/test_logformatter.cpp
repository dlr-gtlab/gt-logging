/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */


#include "test_log_helper.h"

#include <QDateTime>

class LogFormatter : public LogHelperTest
{};

TEST_F(LogFormatter, date)
{
    gtWarning() << "Time Check";

    auto time = QDateTime::currentDateTime().toString("[hh:mm:ss]");

    // This may fail rarely if the logging and the time feching happens
    // in between two seconds
    EXPECT_TRUE(log.contains(time));
}

TEST_F(LogFormatter, filterDefault)
{
    gt::log::Formatter formatter;

    // by default all levels should be included
    EXPECT_TRUE(formatter.filter(gt::log::TraceLevel));
    EXPECT_TRUE(formatter.filter(gt::log::DebugLevel));
    EXPECT_TRUE(formatter.filter(gt::log::InfoLevel));
    EXPECT_TRUE(formatter.filter(gt::log::WarnLevel));
    EXPECT_TRUE(formatter.filter(gt::log::ErrorLevel));
    EXPECT_TRUE(formatter.filter(gt::log::FatalLevel));
}

TEST_F(LogFormatter, filterNone)
{
    gt::log::Formatter formatter;
    formatter.filterAll(false);

    // all levels should be excluded
    EXPECT_FALSE(formatter.filter(gt::log::TraceLevel));
    EXPECT_FALSE(formatter.filter(gt::log::DebugLevel));
    EXPECT_FALSE(formatter.filter(gt::log::InfoLevel));
    EXPECT_FALSE(formatter.filter(gt::log::WarnLevel));
    EXPECT_FALSE(formatter.filter(gt::log::ErrorLevel));
    EXPECT_FALSE(formatter.filter(gt::log::FatalLevel));
}

TEST_F(LogFormatter, filterSelected)
{
    gt::log::Formatter formatter;
    formatter.filterAll(false)
            .filterLevel(gt::log::InfoLevel, true)
            .filterLevel(gt::log::ErrorLevel, true)
            .filterLevel(gt::log::TraceLevel, true);

    // only selected levels should be filtered
    EXPECT_TRUE(formatter.filter(gt::log::TraceLevel));
    EXPECT_FALSE(formatter.filter(gt::log::DebugLevel));
    EXPECT_TRUE(formatter.filter(gt::log::InfoLevel));
    EXPECT_FALSE(formatter.filter(gt::log::WarnLevel));
    EXPECT_TRUE(formatter.filter(gt::log::ErrorLevel));
    EXPECT_FALSE(formatter.filter(gt::log::FatalLevel));

    // now exclude error level but include debug
    formatter.filterLevel(gt::log::ErrorLevel, false)
            .filterLevel(gt::log::DebugLevel, true);

    EXPECT_TRUE(formatter.filter(gt::log::TraceLevel));
    EXPECT_TRUE(formatter.filter(gt::log::DebugLevel));
    EXPECT_TRUE(formatter.filter(gt::log::InfoLevel));
    EXPECT_FALSE(formatter.filter(gt::log::WarnLevel));
    EXPECT_FALSE(formatter.filter(gt::log::ErrorLevel));
    EXPECT_FALSE(formatter.filter(gt::log::FatalLevel));
}

TEST_F(LogFormatter, format_empty)
{
    gt::log::Formatter formatter;
    formatter.setFormat("");

    auto res = formatter.format(gt::log::InfoLevel,   // level
                                std::string{"my_id"}, // id
                                "Hello",              // message
                                std::tm{});           // time

    EXPECT_TRUE(res.empty());
}

TEST_F(LogFormatter, format_1)
{
    auto res = gt::log::Formatter::format("/L \"/M\"",          // format
                                          gt::log::InfoLevel,   // level
                                          std::string{"my_id"}, // id
                                          "Hello",              // message
                                          std::tm{});           // time

    EXPECT_EQ(res, "INFO  \"Hello\"");
}

TEST_F(LogFormatter, format_2)
{
    auto res = gt::log::Formatter::format("/M [/L / [%H:%S:%M] / /I]",// format
                                          gt::log::FatalLevel,        // level
                                          std::string{"my_id"},       // id
                                          "my_message",               // message
                                          std::tm{});                 // time

    EXPECT_EQ(res, "my_message [FATAL / [00:00:00] / my_id]");
}

TEST_F(LogFormatter, format_time)
{
    std::tm time;
    time.tm_hour = 12;
    time.tm_min  = 59;
    time.tm_sec  = 42;
    time.tm_year = 2022 - 1900; // 1900 start time of year
    time.tm_mon  = 11;          // month = december

    gt::log::Formatter formatter;

    formatter.setFormat("%Y "          // year
                        "%B "          // month name
                        "(%S/%M/%H)"); // sec, min, hour

    auto res = formatter.format(gt::log::DebugLevel,  // level
                                std::string{"my_id"}, // id
                                "my_message",         // message
                                time);                // time

    EXPECT_EQ(res, "2022 December (42/59/12)");
}
