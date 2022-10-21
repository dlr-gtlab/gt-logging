#include "gtest/gtest.h"

#include "gt_logging.h"
#include "test_log_helper.h"

class Verbosity : public LogHelperTest
{
    void SetUp() override
    {
        logger.setVerbosity(gt::SILENT);
        LogHelperTest::SetUp();
    }

    void TearDown() override
    {
        logger.setVerbosity(gt::SILENT);
        LogHelperTest::SetUp();
    }
};

TEST_F(Verbosity, logError)
{
    ASSERT_TRUE(log.isEmpty());

    gtError() << "This error should appear";
    EXPECT_TRUE(log.contains("This error should appear"));
}

TEST_F(Verbosity, logErrorVerbose)
{
    ASSERT_TRUE(log.isEmpty());

    gtError().verbose() << "This error should not appear";
    EXPECT_FALSE(log.contains("This error should not appear"));
}

TEST_F(Verbosity, logErrorVerboseHigh)
{
    ASSERT_TRUE(log.isEmpty());

    logger.setVerbosity(gt::EVERYTHING);

    gtError().verbose() << "This verbose error should must appear";
    EXPECT_TRUE(log.contains("This verbose error should must appear"));
}

TEST_F(Verbosity, logErrorVerboseMedium)
{
    ASSERT_TRUE(log.isEmpty());

    logger.setVerbosity(gt::MEDIUM);

    gtError().verbose() << "This verbose error should must not appear";
    EXPECT_FALSE(log.contains("This verbose error should must not appear"));
}

TEST_F(Verbosity, logErrorVerboseMediumSuccess)
{
    ASSERT_TRUE(log.isEmpty());

    logger.setVerbosity(gt::MEDIUM);

    gtError().verbose(gt::MEDIUM) << "This verbose error should must still appear";
    EXPECT_TRUE(log.contains("This verbose error should must still appear"));
}
