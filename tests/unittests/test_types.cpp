
#include "test_log_helper.h"
#include "gt_logging/stl_bindings.h"

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

TEST_F(Types, iomanip_flags)
{
    gtDebug() << std::hex << 15;
    EXPECT_TRUE(log.contains("f"));

    gtDebug() << std::noboolalpha << true << false;
    EXPECT_TRUE(log.contains("1 0"));
};

TEST_F(Types, iomanip_endl)
{
    gtDebug().nospace() << "new" << std::endl << "line";
    EXPECT_TRUE(log.contains("new\nline"));
};

TEST_F(Types, iomanip_setw)
{
    gtDebug() << std::left << std::setw(12) << "0123456789" << "Test";
    EXPECT_TRUE(log.contains("0123456789   Test"));
};

TEST_F(Types, iomanip_setpreciosion)
{
    gtDebug() << std::setprecision(2) << 3.14159;
    EXPECT_TRUE(log.contains("3.1"));
    gtDebug() << std::setprecision(6) << 3.14159;
    EXPECT_FALSE(log.contains("3.1459"));
};

TEST_F(Types, iomanip_setbase)
{
    // checking if this compiles
    gtDebug() << std::setbase(8);
};

enum class Cap { Title, Middle, End };

auto
print(const char* text, double num, Cap c)
{
    gt::log::Stream str;
    str.nospace();

    if (c == Cap::Title)
        str <<
           "----------------------------------------------------\n"
           "|  number  |   iomanip  |      representation      |\n"
           "|----------|------------|--------------------------|\n";

    str << std::left
        << "| " << std::setw(8) << text << " | fixed      | "
        << std::setw(24) << std::fixed        << num << " |\n"
        << "| " << std::setw(8) << text << " | scientific | "
        << std::setw(24) << std::scientific   << num << " |\n"
        << "| " << std::setw(8) << text << " | hexfloat   | "
        << std::setw(24) << std::setprecision(3) << std::hexfloat << num << " |\n"
        << "| " << std::setw(8) << text << " | default    | "
        << std::setw(24) << std::defaultfloat << num << " |\n";

    if (c != Cap::End)
        str <<
            "|----------|------------|--------------------------|\n";
    else
        str <<
            "----------------------------------------------------";

    return str.str();
}

// fancy test to check if most modifers are supported
// source: https://en.cppreference.com/w/cpp/io/manip/fixed
TEST_F(Types, iomanips)
{
    gtInfo().nospace()
        << std::endl
        << print("0.25",     0.25,   Cap::Title)
        << print("0.01",    0.01,    Cap::Middle)
        << print("0.00001", 0.00001, Cap::End);

    QString expected(
"----------------------------------------------------\n"
"|  number  |   iomanip  |      representation      |\n"
"|----------|------------|--------------------------|\n"
"| 0.25     | fixed      | 0.250000                 |\n"
"| 0.25     | scientific | 2.500000e-01             |\n"
#ifdef _WIN32
"| 0.25     | hexfloat   | 0x1.0000000000000p-2     |\n"
#else
"| 0.25     | hexfloat   | 0x1p-2                   |\n"
#endif
"| 0.25     | default    | 0.25                     |\n"
"|----------|------------|--------------------------|\n"
"| 0.01     | fixed      | 0.010000                 |\n"
"| 0.01     | scientific | 1.000000e-02             |\n"
"| 0.01     | hexfloat   | 0x1.47ae147ae147bp-7     |\n"
"| 0.01     | default    | 0.01                     |\n"
"|----------|------------|--------------------------|\n"
"| 0.00001  | fixed      | 0.000010                 |\n"
"| 0.00001  | scientific | 1.000000e-05             |\n"
"| 0.00001  | hexfloat   | 0x1.4f8b588e368f1p-17    |\n"
"| 0.00001  | default    | 1e-05                    |\n"
"----------------------------------------------------");
    EXPECT_TRUE(log.contains(expected));

    gtInfo().nospace()
        << "VS EXPECTED:\n"
        << expected.toStdString();
}

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

TEST_F(Types, memory_unique)
{
    auto u = std::make_unique<int>(42);

    gtError() << u;
    EXPECT_TRUE(log.contains(QString::number((size_t)u.get(), 16)));
    EXPECT_TRUE(log.contains("unique"));
};

TEST_F(Types, memory_shared)
{
    auto s = std::make_shared<int>(42);

    gtError() << s;
    EXPECT_TRUE(log.contains(QString::number((size_t)s.get(), 16)));
    EXPECT_TRUE(log.contains("shared"));
};

TEST_F(Types, std_vector)
{
    auto vec = std::vector<double>{14, 43, 15};
    gtDebug() << vec;
    EXPECT_TRUE(log.contains("(14, 43, 15)"));
}

TEST_F(Types, std_list)
{
    gtWarning() << std::list<float>{0.1f, 0.2f, 0.3f};
    EXPECT_TRUE(log.contains("(0.1, 0.2, 0.3)"));
}

TEST_F(Types, std_map)
{
    std::map<float, std::string> map;
    map.insert({41.2, "ABC"});

    gtWarning() << map;
    EXPECT_TRUE(log.contains(R"(map{(41.2, "ABC")})"));
}

TEST_F(Types, std_multimap)
{
    std::multimap<float, std::string> map;
    map.insert({41.1, "ABC"});
    map.insert({41.2, "Test"});

    gtWarning() << map;
    EXPECT_TRUE(log.contains("multimap{("));
    EXPECT_TRUE(log.contains(R"((41.1, "ABC"))"));
    EXPECT_TRUE(log.contains(R"((41.2, "Test"))"));
    EXPECT_TRUE(log.contains("}"));
}

TEST_F(Types, std_array)
{
    std::array<std::string, 5> array{
        "ABC", "DEF", "TEST", "1234", ""
    };

    gtWarning() << array;
    EXPECT_TRUE(log.contains(R"(["ABC", "DEF", "TEST", "1234", ""])"));
}

TEST_F(Types, std_pair)
{
    std::pair<std::string, int> pair{"test", 42};
    gtWarning() << pair;
    EXPECT_TRUE(log.contains(R"(("test", 42))"));
}

TEST_F(Types, std_tuple)
{
    std::tuple<std::string, int> tuple{"test", 42};
    gtWarning() << tuple;
    EXPECT_TRUE(log.contains(R"(("test", 42))"));

    std::tuple<double, std::pair<bool, float>, int, std::string> nested{
        0.1234, {true, 42.1f}, 42, "test"
    };
    gtWarning() << nested;
    EXPECT_TRUE(log.contains(R"((0.1234, (true, 42.1), 42, "test"))"));
}

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
