#include "gtest/gtest.h"

#define GT_MODULE_ID "BasicLog"

#include "gt_logging.h"
#include "test_log_helper.h"

class BasicLog : public LogHelperTest
{};


TEST_F(BasicLog, logHello)
{
    gtWarning() << "Hello World";
    EXPECT_TRUE(log.contains("Hello World"));
}

TEST_F(BasicLog, checkLogId)
{
    gtWarning() << "Hello World";
    EXPECT_TRUE(log.contains("[BasicLog]"));
}

