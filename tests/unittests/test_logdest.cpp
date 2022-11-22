/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 12.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#include <gtest/gtest.h>
#include "gt_logging.h"
#include<QString>

class DestTest : public testing::Test
{
public:
    DestTest() : logger(gt::log::Logger::instance()) { }

    std::string destid = "my_id";
    gt::log::DestinationPtr dest;
    gt::log::Logger& logger;
};

TEST_F(DestTest, nulldest)
{
    gt::log::DestinationPtr dest{};
    EXPECT_FALSE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));

    // fail if null
    EXPECT_FALSE(logger.addDestination(dest));
}

TEST_F(DestTest, namedDestination)
{
    auto dest = gt::log::makeFunctorDestination(
                    [](std::string const& /*msg*/,
                    gt::log::Level /*lvl*/,
                    gt::log::Details /*details*/){
        // nothing to do here
    });

    ASSERT_FALSE(logger.hasDestination(dest));
    ASSERT_FALSE(logger.hasDestination(destid));

    // add by id
    EXPECT_TRUE(logger.addDestination(destid, dest));

    EXPECT_TRUE(logger.hasDestination(dest));
    EXPECT_TRUE(logger.hasDestination(destid));

    // remove by id
    ASSERT_TRUE(logger.removeDestination(destid));

    EXPECT_FALSE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));
}

TEST_F(DestTest, unnamedDestination)
{
    auto dest = gt::log::makeFunctorDestination(
                    [](std::string const& /*msg*/,
                    gt::log::Level /*lvl*/,
                    gt::log::Details /*details*/){
        // nothing to do here
    });

    ASSERT_FALSE(logger.hasDestination(dest));
    ASSERT_FALSE(logger.hasDestination(destid));

    // add by ptr only
    EXPECT_TRUE(logger.addDestination(dest));
    EXPECT_TRUE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));

    // remove by id -> will fail
    EXPECT_FALSE(logger.removeDestination(destid));
    EXPECT_TRUE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));

    // remove dest by ptr
    EXPECT_TRUE(logger.removeDestination(dest));
    EXPECT_FALSE(logger.hasDestination(dest));
    EXPECT_FALSE(logger.hasDestination(destid));
}
