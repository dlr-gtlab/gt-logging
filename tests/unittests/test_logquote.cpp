/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 11.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

// log with quote
#define GT_LOG_QUOTE

#include "gt_logging_qt_bindings.h"
#include "test_log_helper.h"

class LoQuote : public LogHelperTest
{};

TEST_F(LoQuote, logQString)
{
    gtWarning() << "Hello" << QString{"World"};
    EXPECT_TRUE(log.contains("Hello \"World\""));
}

TEST_F(LoQuote, logQChar)
{
    gtWarning() << 'T' << QChar{'E'} << 'S' << QChar{'T'};
    EXPECT_TRUE(log.contains("T \"E\" S \"T\""));
}

// member methods should be callable as well
TEST_F(LoQuote, callMemberMethod)
{
    gtWarning().nospace() << 'T' << QChar{'E'} << 'S' << QLatin1String{"T"};
    EXPECT_TRUE(log.contains("T\"E\"S\"T\""));
}
