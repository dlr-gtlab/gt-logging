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
        auto dest = gt::log::makeFunctorDestination([this](const std::string& msg, gt::log::Level) {
            log.append(QString::fromStdString(msg));
        });
        ASSERT_TRUE(logger.addDestination("test", dest));

        ASSERT_TRUE(logger.hasDestination(dest));
    }

    void TearDown() override
    {
        logger.removeDestination("test");

        ASSERT_FALSE(logger.hasDestination("test"));
    }


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
