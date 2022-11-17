// custom module id
#define GT_MODULE_ID "BasicLog"

#include "test_log_helper.h"

#include <QDateTime>

class BasicLog : public LogHelperTest
{};

TEST_F(BasicLog, logid)
{
    gtWarning() << "Hello World";
    EXPECT_TRUE(log.contains("Hello World"));
    EXPECT_TRUE(log.contains("[BasicLog]"));
}

TEST_F(BasicLog, customLogid)
{
    gtWarningId("FancyTag") << "Hello World";
    EXPECT_TRUE(log.contains("[FancyTag]"));
}

TEST_F(BasicLog, date)
{
    gtWarning() << "Time Check";

    auto time = QDateTime::currentDateTime().toString("[hh:mm:ss]");

    // This may fail rarely if the logging and the time feching happens
    // in between two seconds
    EXPECT_TRUE(log.contains(time));
}
