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

    logger.addDestination(destid, std::move(dest));
    EXPECT_FALSE(logger.hasDestination(destid));
    EXPECT_EQ(logger.destination(destid), nullptr);
}

TEST_F(DestTest, namedDestination)
{
    auto dest = gt::log::makeFunctorDestination(
                    [](std::string const& /*msg*/,
                    gt::log::Level /*lvl*/,
                    gt::log::Details /*details*/){
        // nothing to do here
    });

    auto* ptr = dest.get();

    ASSERT_FALSE(logger.hasDestination(destid));

    // add by id
    EXPECT_TRUE(logger.addDestination(destid, std::move(dest)));

    EXPECT_TRUE(logger.hasDestination(destid));
    EXPECT_EQ(logger.destination(destid), ptr);

    // remove by id
    ASSERT_TRUE(logger.removeDestination(destid));

    EXPECT_FALSE(logger.hasDestination(destid));

    EXPECT_EQ(logger.destination(destid), nullptr);
}
