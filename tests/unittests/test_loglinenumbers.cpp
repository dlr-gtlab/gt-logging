// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

// log line number
#define GT_LOG_LINE_NUMBERS

#include "test_log_helper.h"
#include "gt_logging/qt_bindings.h"

class LogLineNo : public LogHelperTest
{};

TEST_F(LogLineNo, logLineNumbers)
{
    gtWarning() << QString{"Hello"} << QString{"World"};
    EXPECT_TRUE(log.contains("Hello World"));
    EXPECT_TRUE(log.contains(__FILE__));
    EXPECT_TRUE(log.contains(QString::number(__LINE__ - 3)));
}

// member methods should be callable as well
TEST_F(LogLineNo, callMemberMethod)
{
    gtWarning().nospace() << "He" << "llo" << QString{"World"};
    EXPECT_TRUE(log.contains("HelloWorld"));
    EXPECT_TRUE(log.contains(__FILE__));
    EXPECT_TRUE(log.contains(QString::number(__LINE__ - 3)));
}

TEST_F(LogLineNo, verbosity_issue43)
{
    gt::log::Logger::instance().setVerbosity(gt::log::Silent);
    
    gtWarning().verbose().nospace() << "Test";
    EXPECT_TRUE(log.isEmpty());
}
