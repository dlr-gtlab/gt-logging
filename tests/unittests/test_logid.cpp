// custom module id
#define GT_MODULE_ID "BasicLog"

#include "test_log_helper.h"

class LogId : public LogHelperTest
{};

TEST_F(LogId, logid)
{
    gtWarning() << "Hello World";
    EXPECT_TRUE(log.contains("Hello World"));
    EXPECT_TRUE(log.contains("[BasicLog]"));
}

TEST_F(LogId, customLogid)
{
    gtWarningId("FancyTag") << "Hello World";
    EXPECT_TRUE(log.contains("[FancyTag]"));
}
