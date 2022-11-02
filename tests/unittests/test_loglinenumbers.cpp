/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 2.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

// log line number
#define GT_LOG_LINE_NUMBERS

#include "test_log_helper.h"

class BasicLog : public LogHelperTest
{};

TEST_F(BasicLog, logLineNumbers)
{
    gtWarning() << QString{"Hello"} << QString{"World"};
    EXPECT_TRUE(log.contains("Hello World"));
    EXPECT_TRUE(log.contains(__FILE__));
    EXPECT_TRUE(log.contains(QString::number(__LINE__-3)));
}
