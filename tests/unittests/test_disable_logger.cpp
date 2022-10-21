#include "gtest/gtest.h"

#include "gt_logging.h"
#include "QsLogDisableForThisFile.h"

class TestDisableLogger : public testing::Test
{};

TEST_F(TestDisableLogger, gtError)
{
    gtError() << "This should only use gtDebug ...";
}

TEST_F(TestDisableLogger, gtErrorId)
{
    gtErrorId("LogTest") << "This should only use gtDebug ...";
}
