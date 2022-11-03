#ifndef TEST_LOG_HELPER_H
#define TEST_LOG_HELPER_H

#include <gtest/gtest.h>
#include "gt_logging.h"

// log helper
class LogHelperTest : public testing::Test
{
public:
    LogHelperTest() : logger(gt::log::Logger::instance())
    {
        dest = gt::log::DestinationFactory::MakeFunctorDestination(logToStr);
        logger.addDestination(dest);
    }

    void SetUp() override
    {
        log.clear();
        ASSERT_TRUE(log.isEmpty());
    }

    void TearDown() override
    {
        qDebug() << log;
        log.clear();
        logger.removeDestination(dest);
    }

    static void logToStr(const QString& msg, gt::log::Level)
    {
        log.append(msg);
    }

    gt::log::DestinationPtr dest;
    gt::log::Logger& logger;
    static QString log;
};

// QObject sample object (must apperently be in header for Q_OBJECT to work)
class MyQObject : public QObject
{
    Q_OBJECT

public:

    enum TestEnum
    {
        FirstEntry = 0,
        SecondEntry,
        ThirdENtry
    };
    Q_ENUM(TestEnum);

    enum Flag
    {
        A = 1,
        B = 2,
        C = 4,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    MyQObject();
};


#endif // TEST_LOG_HELPER_H
