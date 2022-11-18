
#include "test_log_helper.h"

// test fixture
class Types : public LogHelperTest {};

TEST_F(Types, Enum)
{
    enum AB
    {
        A = 42,
        B = 96
    };

    AB e = A;

    gtError() << e << AB{B};
    EXPECT_TRUE(log.contains("42"));
    EXPECT_TRUE(log.contains("96"));
};

TEST_F(Types, QEnum)
{
    auto e = MyQObject::FirstEntry;

    gtError() << e;
    EXPECT_TRUE(log.contains("0"));
};

TEST_F(Types, QFlags)
{
    MyQObject::Flags f = {MyQObject::A | MyQObject::C};

    gtError() << f;
    EXPECT_TRUE(log.contains("5"));
};

TEST_F(Types, POD_other)
{
    void* v = (void*)12345;
    uint* uiptr = (uint*)12345;
    std::nullptr_t t{};
    bool b = true;

    gtError() << v;
    EXPECT_TRUE(log.contains(QString::number((size_t)v, 16)));
    gtError() << uiptr;
    EXPECT_TRUE(log.contains(QString::number((size_t)uiptr, 16)));
    gtError() << t;
    EXPECT_TRUE(log.contains("(nullptr)"));
    gtInfo() << b;
    EXPECT_TRUE(log.contains("true"));
}

TEST_F(Types, POD_chars)
{
    char c = 'C';
    const char* cs = "Hello";
    char16_t wc = 'W';
    char32_t wc32 = 'L';

    gtInfo() << c;
    EXPECT_TRUE(log.contains(c));
    gtInfo() << cs;
    EXPECT_TRUE(log.contains(cs));
    gtInfo() << wc;
    EXPECT_TRUE(log.contains("u'" + QString::number(wc)));
    gtInfo() << wc32;
    EXPECT_TRUE(log.contains("U'" + QString::number(wc32)));
}

TEST_F(Types, POD_ints)
{
    signed short ssi = -127;
    unsigned short usi = 128;
    signed int si = -42;
    unsigned int ui = 41;
    signed long li = -1234;
    unsigned long uli = 4321;
    qint64 qi64 = 1029384756;
    quint64 qui64 = 5647382910;

    gtInfo() << ssi;
    EXPECT_TRUE(log.contains(QString::number(ssi)));
    gtInfo() << usi;
    EXPECT_TRUE(log.contains(QString::number(usi)));
    gtInfo() << si;
    EXPECT_TRUE(log.contains(QString::number(si)));
    gtInfo() << ui;
    EXPECT_TRUE(log.contains(QString::number(ui)));
    gtInfo() << li;
    EXPECT_TRUE(log.contains(QString::number(li)));
    gtInfo() << uli;
    EXPECT_TRUE(log.contains(QString::number(uli)));
    gtInfo() << qi64;
    EXPECT_TRUE(log.contains(QString::number(qi64)));
    gtInfo() << qui64;
    EXPECT_TRUE(log.contains(QString::number(qui64)));
}

TEST_F(Types, POD_float)
{
    float f  = 0.123f;
    double d = 3.1415;

    gtInfo() << f;
    EXPECT_TRUE(log.contains(QString::number(f)));
    gtInfo() << d;
    EXPECT_TRUE(log.contains(QString::number(d)));
}

TEST_F(Types, Strings)
{
    std::string s = "MyStdString";

    // also test for const
    gtError() << s << qAsConst(s);
    EXPECT_TRUE(log.contains(s.c_str()));
}
