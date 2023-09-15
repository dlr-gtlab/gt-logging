/* GTlab - Gas Turbine laboratory
 * copyright 2009-2023 by DLR
 *
 *  Created on: 15.9.2023
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#define GT_MODULE_ID "BasicLog"

#include "test_log_helper.h"

#include <unordered_set>

class LogOnce : public LogHelperTest
{};

TEST_F(LogOnce, global_cache)
{

    ASSERT_EQ(log.count("log once"), 0);

    gtLogOnce(Debug) << "log once";
    EXPECT_TRUE(log.contains("BasicLog")); // check that default id works
    EXPECT_EQ(log.count("log once"), 1);

    gtLogOnce(Debug) << "log once";
    gtLogOnce(Debug) << "log once";
    EXPECT_EQ(log.count("log once"), 1);

    gtLogOnce(Info) << "log once";
    gtLogOnce(Info) << "log once";
    gtLogOnce(Info) << "log once";
    EXPECT_EQ(log.count("log once"), 2);

    gtLogOnceId(Info, "Test") << "log once";

    EXPECT_EQ(log.count("log once"), 3);
    EXPECT_TRUE(log.contains("Test"));

    gtLogOnce(Info) << "test";
    EXPECT_EQ(log.count("test"), 1);
    EXPECT_EQ(log.count("log once"), 3);

}

TEST_F(LogOnce, local_cache)
{
    struct Cache
    {
        void append(gt::log::hash_t hash) { cache.insert(hash); }

        void clear() { cache.clear(); }

        bool find(gt::log::hash_t hash) { return cache.find(hash) != cache.end(); }

        std::unordered_set<gt::log::hash_t> cache;
    };

    Cache localCache;

    ASSERT_EQ(log.count("log once"), 0);

    gtLogOnce(Info, localCache) << "log once";
    EXPECT_TRUE(log.contains("BasicLog")); // check that default id works
    EXPECT_EQ(log.count("log once"), 1);
    EXPECT_EQ(localCache.cache.size(), 1);

    gtLogOnce(Info, localCache) << "log once";
    gtLogOnce(Info, localCache) << "log once";
    EXPECT_EQ(log.count("log once"), 1);
    EXPECT_EQ(localCache.cache.size(), 1);

    gtLogOnceId(Info, "TEST", localCache) << "log once";
    EXPECT_EQ(log.count("log once"), 2);
    EXPECT_EQ(localCache.cache.size(), 2);

    gtLogOnceId(Info, "TEST", localCache) << "log once";
    gtLogOnceId(Info, "TEST", localCache) << "log once";
    EXPECT_EQ(log.count("log once"), 2);
    EXPECT_EQ(localCache.cache.size(), 2);
}
