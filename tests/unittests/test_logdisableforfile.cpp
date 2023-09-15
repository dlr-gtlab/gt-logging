#include "test_log_helper.h"
#include "gt_logdisablelogforfile.h"

class TestDisableLogging : public LogHelperTest
{};

TEST_F(TestDisableLogging, gtError)
{
    gtError() << "This should not be logged...";
    EXPECT_TRUE(log.isEmpty());
}

TEST_F(TestDisableLogging, gtErrorId)
{
    gtErrorId("LogTest") << "This should not be logged either...";
    EXPECT_TRUE(log.isEmpty());
}

TEST_F(TestDisableLogging, logOnce)
{
    gtLogOnce(Info).nospace() << "This should not be logged";
    gtLogOnceId(Info).nospace() << "This should not be logged too";
    EXPECT_TRUE(log.isEmpty());
}

// member methods should be callable as well
TEST_F(TestDisableLogging, callMemberMethod)
{
    gtError().nospace() << "This should not be logged too";
    EXPECT_TRUE(log.isEmpty());
}
