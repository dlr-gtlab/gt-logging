/* GTlab - Gas Turbine laboratory
 * copyright 2009-2023 by DLR
 *
 *  Created on: 15.9.2023
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#include "test_log_helper.h"

class LogOnce : public LogHelperTest
{};

TEST_F(LogOnce, logOnce)
{
    gtLogOnce(Debug) << "log once";
}
