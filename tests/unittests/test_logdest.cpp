// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>
#include "gt_logging.h"

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

TEST_F(DestTest, destinations)
{
    auto destinations = logger.destinationIds();
    ASSERT_EQ(destinations.size(), 1);
    EXPECT_EQ(destinations.front(), "console");

    auto dest = gt::log::makeFunctorDestination(
        [](std::string const& /*msg*/,
           gt::log::Level /*lvl*/,
           gt::log::Details /*details*/){
        // nothing to do here
    });

    EXPECT_TRUE(logger.addDestination(destid, std::move(dest)));

    destinations = logger.destinationIds();
    ASSERT_EQ(destinations.size(), 2);
    EXPECT_EQ(destinations.back(), destid);
}
