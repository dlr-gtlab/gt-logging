
#include "test_log_helper.h"

class Verbosity : public LogHelperTest
{
    void SetUp() override
    {
        logger.setVerbosity(gt::log::Silent);
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
    gtError().medium() << "This error should not appear";
    EXPECT_FALSE(log.contains("This error should not appear"));
}

TEST_F(Verbosity, logErrorVerboseHigh)
{
    ASSERT_TRUE(log.isEmpty());

    logger.setVerbosity(gt::log::Everything);

    gtError().verbose() << "This verbose error must appear";
    gtError().medium() << "42";
    EXPECT_TRUE(log.contains("This verbose error must appear"));
    EXPECT_TRUE(log.contains("42"));
}

TEST_F(Verbosity, logErrorVerboseMedium)
{
    ASSERT_TRUE(log.isEmpty());

    logger.setVerbosity(gt::log::Medium);

    gtError().verbose() << "This verbose error must not appear";
    EXPECT_FALSE(log.contains("This verbose error must not appear"));
}

TEST_F(Verbosity, logErrorVerboseMediumSuccess)
{
    ASSERT_TRUE(log.isEmpty());

    logger.setVerbosity(gt::log::Medium);

    gtError().verbose(gt::log::Medium) << "This verbose error should appear";
    EXPECT_TRUE(log.contains("This verbose error should appear"));
}

TEST_F(Verbosity, logErrorVerboseMediumSuccess2)
{
    ASSERT_TRUE(log.isEmpty());

    logger.setVerbosity(gt::log::Medium);

    gtError().medium() << "This verbose error should still appear";
    EXPECT_TRUE(log.contains("This verbose error should still appear"));
}

TEST_F(Verbosity, test)
{
    std::tm time{};
    time.tm_hour = 12;
    time.tm_min  = 59;
    time.tm_sec  = 42;
    time.tm_year = 2022 - 1900; // 1900 start time of year
    time.tm_mon  = 11;          // month = december

    std::string id = "my_id";

    std::string msg = "Hello World";

    gt::log::Level lvl = gt::log::InfoLevel;

    std::cout << gt::log::format("%1 [%3] [%2] %4 %5", lvl, id, time, msg, "(test)") << std::endl;
}
