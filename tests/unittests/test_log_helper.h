#ifndef TEST_LOG_HELPER_H
#define TEST_LOG_HELPER_H

#include <gtest/gtest.h>
#include "gt_logging.h"

class LogHelperTest : public testing::Test
{
public:
    LogHelperTest() : logger(QsLogging::Logger::instance())
    {
        logger.addDestination(QsLogging::DestinationFactory::MakeFunctorDestination(logToStr));
    }

    void SetUp() override
    {
        log.clear();
    }

    void TearDown() override
    {
        log.clear();
    }

    static void logToStr(const QString& msg, QsLogging::Level)
    {
        log.append(msg);
    }

    QsLogging::Logger& logger;
    static QString log;
};

#endif // TEST_LOG_HELPER_H
