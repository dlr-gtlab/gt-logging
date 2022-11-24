/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 11.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

// log with quote
#define GT_LOG_QUOTE

#include "test_log_helper.h"
#include "gt_logging/qt_bindings.h"

class LogQuote : public LogHelperTest
{};

TEST_F(LogQuote, logQString)
{
    gtWarning() << "Hello" << QString{"World"};
    EXPECT_TRUE(log.contains("Hello \"World\""));
}

TEST_F(LogQuote, logQChar)
{
    gtWarning() << 'T' << QChar{'E'} << 'S' << QChar{'T'};
    EXPECT_TRUE(log.contains("T \"E\" S \"T\""));
}

// member methods should be callable as well
TEST_F(LogQuote, callMemberMethod)
{
    gtWarning().nospace() << 'T' << QChar{'E'} << 'S' << QLatin1String{"T"};
    EXPECT_TRUE(log.contains("T\"E\"S\"T\""));
}
