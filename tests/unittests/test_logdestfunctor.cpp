/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 12.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#include "test_log_helper.h"

#include <QCoreApplication>

class LogFunctor : public testing::Test
{
public:

    LogFunctor() : logger(gt::log::Logger::instance()) { }

    gt::log::DestinationPtr dest;
    gt::log::Logger& logger;
};

void myLogFunction(std::string const& /*s*/, gt::log::Level /*l*/)
{
    // nothing to do here
}

TEST_F(LogFunctor, function_ptr)
{
    std::string destid = "functor_ptr";
    dest = gt::log::makeFunctorDestination(myLogFunction);

    EXPECT_TRUE(dest->isValid());
    EXPECT_FALSE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));

    EXPECT_TRUE(logger.addDestination(destid, dest));
    EXPECT_TRUE(logger.hasDestination(dest));
    EXPECT_TRUE(logger.hasDestination(destid));

    EXPECT_TRUE(logger.removeDestination(destid));
    EXPECT_FALSE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));
}

TEST_F(LogFunctor, function_lambda)
{
    std::string destid = "functor_lambda";
    dest = gt::log::makeFunctorDestination([](
                                           std::string  const&,
                                           gt::log::Level){});

    EXPECT_TRUE(dest->isValid());
    EXPECT_FALSE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));

    EXPECT_TRUE(logger.addDestination(destid, dest));
    EXPECT_TRUE(logger.hasDestination(dest));
    EXPECT_TRUE(logger.hasDestination(destid));

    EXPECT_TRUE(logger.removeDestination(destid));
    EXPECT_FALSE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));
}
