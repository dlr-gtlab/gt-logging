#include "gtest/gtest.h"

#include "gt_logging.h"

#include <QCoreApplication>

// init at startup. Prints any output to stdout/stderr
static auto init_logger_once = [](){
    auto& logger = gt::log::Logger::instance();
    logger.addDestination("output", gt::log::makeDebugOutputDestination());
    return 0;
}();

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    int retval = RUN_ALL_TESTS();

    return retval;
}
