// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>
#include "gt_logging.h"

class DestTest : public testing::Test
{
public:
    DestTest() : logger(gt::log::Logger::instance()) { }

    void TearDown() override
    {
//        logger.removeDestination(destid);
    }

    std::string destid = "my_id";
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

class DestLogToTest : public testing::Test
{
public:
    DestLogToTest() : logger(gt::log::Logger::instance()) { }

    void TearDown() override
    {
        logger.removeDestination(destid1);
        logger.removeDestination(destid2);
    }

    std::string destid1 = "1";
    std::string destid2 = "2";
    gt::log::Logger& logger;
};


TEST_F(DestLogToTest, logTo)
{
    std::string log1, log2;

    auto dest1 = gt::log::makeFunctorDestination(
        [&log1](std::string const& msg,
               gt::log::Level /*lvl*/,
               gt::log::Details /*details*/){
        log1 += msg;
    });
    auto dest2 = gt::log::makeFunctorDestination(
        [&log2](std::string const& msg,
               gt::log::Level /*lvl*/,
               gt::log::Details /*details*/){
        log2 += msg;
    });

    logger.addDestination(destid1, std::move(dest1));
    logger.addDestination(destid2, std::move(dest2));

    auto destPtr1 = logger.destination("1");
    ASSERT_TRUE(destPtr1);

    // this logs to all destinations
    gtDebug() << "test";

    EXPECT_EQ(log1.find("test"), 0);
    EXPECT_EQ(log2.find("test"), 0);
    log1.clear();
    log2.clear();

    // this logs only to the desired destination
    gtLogTo(Debug, destPtr1) << "bla";

    EXPECT_EQ(log1.find("bla"), 0);
    EXPECT_EQ(log2.find("bla"), std::string::npos);
    log1.clear();
    log2.clear();

    // invalid destination
    ASSERT_NO_FATAL_FAILURE(gtLogTo(Debug, nullptr) << "test");
}
