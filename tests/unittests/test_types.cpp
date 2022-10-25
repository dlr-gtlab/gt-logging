#include "gtest/gtest.h"
#include "gt_logging.h"

#include "test_log_helper.h"

class Types : public LogHelperTest
{};

TEST_F(Types, QStringList)
{
    ASSERT_TRUE(log.isEmpty());

    gtWarning() << QStringList{"Hallo", "Welt"};
    EXPECT_TRUE(log.contains("(Hallo, Welt)"));
}

TEST_F(Types, QHash)
{
    ASSERT_TRUE(log.isEmpty());

    QHash<QString, int> hash;
    hash.insert("a", 1);
    hash.insert("b", 2);

    gtError() << hash;
    EXPECT_TRUE(log.contains("QHash")); // note, the order of args is random
}

TEST_F(Types, QVariant)
{
    ASSERT_TRUE(log.isEmpty());

    gtError() << QVariant("AVariant");
    EXPECT_TRUE(log.contains("AVariant"));
}

TEST_F(Types, Enum)
{
    enum AB
    {
        A = 42,
        B
    };

    ASSERT_TRUE(log.isEmpty());
    AB e = A;

    gtError() << e;
    EXPECT_TRUE(log.contains("42"));
};
