/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */


#include "qdebug.h"
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
    qDebug() << time;
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

TEST_F(LogFormatter, standalone_format_1)
{
    auto res = gt::log::format("%1 \"%2\"", gt::log::InfoLevel, "Hello");

    EXPECT_EQ(res, "INFO  \"Hello\"");
}

TEST_F(LogFormatter, standalone_format_2)
{
    auto res = gt::log::format("%1 [%2 / %4 / %3]",
                               "my_message",
                               gt::log::FatalLevel,
                               "my_id",
                               std::tm{});

    EXPECT_EQ(res, "my_message [FATAL / 00:00:00 / my_id]");
}

TEST_F(LogFormatter, standalone_time_format)
{
    std::tm time{};
    time.tm_hour = 12;
    time.tm_min  = 59;
    time.tm_sec  = 42;
    time.tm_year = 2022 - 1900; // 1900 start time of year
    time.tm_mon  = 11;          // month = december

    auto res = gt::log::formatTime(time,
                                   "%Y "          // year
                                   "%B "          // month name
                                   "(%S/%M/%H)"); // day time

    EXPECT_EQ(res, "2022 December (42/59/12)");
}

TEST_F(LogFormatter, format_empty)
{
    gt::log::Formatter formatter([](std::string const&,
                                 gt::log::Level,
                                 gt::log::Details const&){
        return std::string{};
    });

    auto res = formatter.format(
       "Hello",              // message
       gt::log::InfoLevel,   // level
       gt::log::Details
       {
           std::string{"my_id"}, // id
           std::tm{}             // time
       }
    );

    EXPECT_TRUE(res.empty());
}

TEST_F(LogFormatter, format)
{
    std::tm time{};
    time.tm_hour = 12;
    time.tm_min  = 59;
    time.tm_sec  = 42;
    time.tm_year = 2077 - 1900; // 1900 start time of year
    time.tm_mon  = 0;           // month = january

    gt::log::Formatter formatter([](std::string const& msg,
                                 gt::log::Level lvl,
                                 gt::log::Details const& dts){
        using gt::log::formatTime;
        return gt::log::format("[%1 | %2] %3: %4", formatTime(dts.time, "%B - %Y"), lvl, dts.id, msg);
    });

    auto res = formatter.format(
       "Hello",                  // message
       gt::log::TraceLevel,      // level
       gt::log::Details
       {
           std::string{"my_id"}, // id
           time                  // time
       }
    );

    EXPECT_EQ(res, "[January - 2077 | TRACE] my_id: Hello");
}
