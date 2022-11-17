/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 16.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */


#include "test_log_helper.h"

#include "gt_logging_qt_bindings.h"

class LogStateSaver : public LogHelperTest
{};

TEST_F(LogStateSaver, stateSaver)
{
    logger.setVerbosity(gt::log::Silent);

    gt::log::Stream stream;

    stream.space() << "A" << "B" << "C";
    {
        gt::log::StreamStateSaver saver{stream};
        // change flags
        stream.nospace() << "D" << "E" << "F";
        stream.quote()   << QString{"quotes"};
        // change verbosity
        stream.verbose() << "This wont log";
        stream << "this wont too";
    }
    // state restored
    stream << "but" << "this" << "will";

    EXPECT_EQ(stream.str(), std::string("A B C DEF\"quotes\"but this will "));
}
