// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

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

struct DummyDestination : public gt::log::FormattedDestination
{
    void write(std::string const&, gt::log::Level) {}
    std::string type() const { return "dummy"; }
};

TEST_F(LogFormatter, filterDefault)
{
    DummyDestination formattedDest;

    // by default all levels should be included
    EXPECT_TRUE(formattedDest.filter(gt::log::TraceLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::DebugLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::InfoLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::WarningLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::ErrorLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::FatalLevel));
}

TEST_F(LogFormatter, filterNone)
{
    DummyDestination formattedDest;
    formattedDest.filterAll(false);

    // all levels should be excluded
    EXPECT_FALSE(formattedDest.filter(gt::log::TraceLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::DebugLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::InfoLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::WarningLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::ErrorLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::FatalLevel));
}

TEST_F(LogFormatter, filterSelected)
{
    DummyDestination formattedDest;
    formattedDest.filterAll(false)
            .filterLevel(gt::log::InfoLevel, true)
            .filterLevel(gt::log::ErrorLevel, true)
            .filterLevel(gt::log::TraceLevel, true);

    // only selected levels should be filtered
    EXPECT_TRUE(formattedDest.filter(gt::log::TraceLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::DebugLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::InfoLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::WarningLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::ErrorLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::FatalLevel));

    // now exclude error level but include debug
    formattedDest.filterLevel(gt::log::ErrorLevel, false)
            .filterLevel(gt::log::DebugLevel, true);

    EXPECT_TRUE(formattedDest.filter(gt::log::TraceLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::DebugLevel));
    EXPECT_TRUE(formattedDest.filter(gt::log::InfoLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::WarningLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::ErrorLevel));
    EXPECT_FALSE(formattedDest.filter(gt::log::FatalLevel));
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

TEST_F(LogFormatter, messageOnly)
{
    gt::log::Formatter f{ gt::log::Formatter::MessageOnly() };

    auto trace = f.format("my fancy trace message", gt::log::TraceLevel, {});
    EXPECT_EQ(trace, "my fancy trace message");

    auto info = f.format("my fancy message", gt::log::InfoLevel, {});
    EXPECT_EQ(info, "my fancy message");

    auto warn = f.format("my fancy message", gt::log::WarningLevel, {});
    EXPECT_EQ(warn, "WARN : my fancy message");

    auto err = f.format("my fancy message", gt::log::ErrorLevel, {});
    EXPECT_EQ(err, "ERROR: my fancy message");
}
