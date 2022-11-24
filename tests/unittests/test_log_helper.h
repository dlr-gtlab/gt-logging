#ifndef TEST_LOG_HELPER_H
#define TEST_LOG_HELPER_H

#include <gtest/gtest.h>
#include "gt_logging.h"

#include <QString>
#include <QObject>

// log helper
class LogHelperTest : public testing::Test
{
public:
    LogHelperTest() : logger(gt::log::Logger::instance()) { }

    void SetUp() override
    {
        auto dest = gt::log::makeFunctorDestination(
                        [this](std::string const& msg,
                        gt::log::Level level,
                        gt::log::Details details){
            log.append(
                QStringLiteral("%1 [%2] [%3] %4")
                        .arg(level)
                        .arg(details.id.c_str(),
                             gt::log::formatTime(details.time).c_str(),
                             msg.c_str())
            );
        });

        ASSERT_TRUE(logger.addDestination(destid, std::move(dest)));
        ASSERT_TRUE(logger.hasDestination(destid));
    }

    void TearDown() override
    {
        logger.removeDestination(destid);

        ASSERT_FALSE(logger.hasDestination(destid));
    }

    std::string destid = "LogHelperTest";
    gt::log::Logger& logger;
    QString log;
};

// QObject sample object (must be in header for Q_OBJECT to work)
class MyQObject : public QObject
{
    Q_OBJECT

public:

    enum TestEnum
    {
        FirstEntry = 0,
        SecondEntry,
        ThirdEntry
    };
    Q_ENUM(TestEnum);

    enum Flag
    {
        A = 1,
        B = 2,
        C = 4,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    MyQObject() = default;
};

#endif // TEST_LOG_HELPER_H
