
#include "test_log_helper.h"
#include "gt_logging_qt_bindings.h"

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QModelIndex>

#include <QWidget>
#include <QJsonValue>


// test fixture
class Types : public LogHelperTest {};

// custom struct
struct MyDebugObject {};

gt::log::Stream& operator<<(gt::log::Stream& d, MyDebugObject const& /*obj*/)
{
    return d << "MyDebugObject";
}

std::ostream& operator<<(std::ostream& d, MyDebugObject const& /*obj*/)
{
    return d << "this_wont_be_used";
}

QDebug& operator<<(QDebug& d, MyDebugObject const& /*obj*/)
{
    return d << "this_wont_be_used_either";
}

TEST_F(Types, QStringList)
{
    gtWarning() << QStringList{"Hallo", "Welt"};
    EXPECT_TRUE(log.contains("(Hallo, Welt)"));
}

TEST_F(Types, QList)
{
    gtWarning() << QList<int>{14, 43, 15};
    EXPECT_TRUE(log.contains("(14, 43, 15)"));
}

TEST_F(Types, QHash)
{
    QHash<QString, int> hash;
    hash.insert("a", 1);
    hash.insert("b", 2);

    gtError() << hash;

    // note, the order of args is random
    EXPECT_TRUE(log.contains("QHash("));
    EXPECT_TRUE(log.contains("(a, 1)"));
    EXPECT_TRUE(log.contains("(b, 2)"));
}

TEST_F(Types, QMap)
{
    QMap<QString, int> map;
    map.insert("a", 11);
    map.insert("b", 22);

    gtError() << map;
    // note, the order of args is random
    EXPECT_TRUE(log.contains("QMap("));
    EXPECT_TRUE(log.contains("(a, 11)"));
    EXPECT_TRUE(log.contains("(b, 22)"));
}

TEST_F(Types, QSet)
{
    QSet<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(33);

    gtError() << set;
    // note, the order of args is random
    EXPECT_TRUE(log.contains("QSet("));
    EXPECT_TRUE(log.contains("1"));
    EXPECT_TRUE(log.contains("2"));
    EXPECT_TRUE(log.contains("33"));
}

TEST_F(Types, QPair)
{
    QPair<QString, int> pair{"test", 42};
    gtWarning() << pair;
    EXPECT_TRUE(log.contains("QPair(test,42)"));
}

TEST_F(Types, std_vector)
{
    gtWarning() << std::vector<double>{14, 43, 15};
    EXPECT_TRUE(log.contains("std::vector(14, 43, 15)"));
}

TEST_F(Types, std_list)
{
    // this doesnt work because we defined the operator<< only for gt::log::Stream
//    gtWarning() << std::list<MyDebugObject>{};

    gtWarning() << std::list<float>{};
    EXPECT_TRUE(log.contains("std::list()"));
}

TEST_F(Types, std_map)
{
    std::map<float, QString> map;
    map.insert({41.1, "ABC"});

    gtWarning() << map;
    EXPECT_TRUE(log.contains("std::map("));
    EXPECT_TRUE(log.contains("std::pair(41.1,ABC)"));
}

TEST_F(Types, std_multimap)
{
    std::multimap<float, QString> map;
    map.insert({41.1, "ABC"});
    map.insert({41.1, "Test"});

    gtWarning() << map;
    EXPECT_TRUE(log.contains("std::multimap("));
    EXPECT_TRUE(log.contains("std::pair(41.1,ABC)"));
    EXPECT_TRUE(log.contains("std::pair(41.1,Test)"));
}

TEST_F(Types, std_pair)
{
    std::pair<QString, int> pair{"test", 42};
    gtWarning() << pair;
    EXPECT_TRUE(log.contains("std::pair(test,42)"));
}

TEST_F(Types, QSharedPointer)
{
    QSharedPointer<int> ptr{new int{42}};

    gtWarning() << ptr;
    EXPECT_TRUE(log.contains("QSharedPointer("));
    EXPECT_TRUE(log.contains(QString::number((size_t)ptr.data(), 16)));
}
TEST_F(Types, QContiguousCache)
{
    QContiguousCache<int> cache;

    gtWarning() << cache;
    EXPECT_TRUE(log.contains("QContiguousCache()"));
}

TEST_F(Types, QVariant)
{
    gtError() << QVariant("A_Variant");
    EXPECT_TRUE(log.contains("QVariant(QString, A_Variant)"));

    gtError() << QVariant(42);
    EXPECT_TRUE(log.contains("QVariant(int, 42)"));
}

TEST_F(Types, QObject)
{
    MyQObject obj;
    obj.setObjectName("MyObjectName");

    gtError() << &obj;

    EXPECT_TRUE(log.contains("MyObjectName"));
    EXPECT_TRUE(log.contains(QString::number((size_t)&obj, 16), Qt::CaseInsensitive));
}

TEST_F(Types, MyDebugObject)
{
    MyDebugObject obj;
    gtError() << obj;
    gtError() << &obj;

    EXPECT_TRUE(log.contains("MyDebugObject"));
    EXPECT_TRUE(log.contains(QString::number((size_t)&obj, 16), Qt::CaseInsensitive));
}

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
    EXPECT_TRUE(log.contains("MyQObject::FirstEntry"));
};

TEST_F(Types, QFlags)
{
    MyQObject::Flags f = {MyQObject::A | MyQObject::C};

    gtError() << f;
    EXPECT_TRUE(log.contains("QFlags(0x1|0x4)"));
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
    QChar qc = 'Q';

    gtInfo() << c;
    EXPECT_TRUE(log.contains(c));
    gtInfo() << cs;
    EXPECT_TRUE(log.contains(cs));
    gtInfo() << wc;
    EXPECT_TRUE(log.contains("u'" + QString::number(wc)));
    gtInfo() << wc32;
    EXPECT_TRUE(log.contains("U'" + QString::number(wc32)));
    gtInfo() << qc;
    EXPECT_TRUE(log.contains(qc));
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
    QString qstr = "MyFancyString";
    QByteArray qba = "MyPreciousBytes";
    QLatin1String qlt1{"MyLatinString"};
    std::string s = "MyStdString";

    // also test for const
    gtError() << qstr << qAsConst(qstr);
    EXPECT_TRUE(log.contains(qstr));
    gtError() << qba << qAsConst(qba);
    EXPECT_TRUE(log.contains(qba));
    gtError() << qlt1 << qAsConst(qlt1);
    EXPECT_TRUE(log.contains(qlt1));
    gtError() << s << qAsConst(s);
    EXPECT_TRUE(log.contains(s.c_str()));
    gtDebug() << QObject::tr("Test");
    EXPECT_TRUE(log.contains("Test"));
}

TEST_F(Types, QStringRef)
{
    QString qstr = "MyFancyString";
    QStringRef qstrr = &qstr;

    gtError() << qstrr;
    EXPECT_TRUE(log.contains(qstr));
}

TEST_F(Types, StringViews)
{
    QTextStreamFunction qstrf{};
    gtError() << qstrf;
    EXPECT_TRUE(log.isEmpty());

    QString qstr = "MyFancyString";
    QStringView qstrv{qstr};

    auto myStrFunc = [](QTextStream& stream) -> QTextStream&{
        return stream << "Test";
    };
    QTextStreamFunction qstrf2{myStrFunc};

    gtError() << qstrv;
    EXPECT_TRUE(log.contains(qstr));
    gtError() << qstrf2;
    EXPECT_TRUE(log.contains("Test"));
}

TEST_F(Types, QPoints)
{
    QModelIndex idx;
    QPoint pt{10, 11};
    QPointF ptf{12.1, 13.2};
    QVector2D v2d{14.1f, 15};
    QVector3D v3d{16.1f, 17, 18};
    QVector4D v4d{19.1f, 20, 21, 22};

    gtError() << idx;
    EXPECT_TRUE(log.contains("QModelIndex(-1,-1,0x0,QObject(0x0))"));
    gtError() << pt;
    EXPECT_TRUE(log.contains("QPoint(10,11)"));
    gtError() << ptf;
    EXPECT_TRUE(log.contains("QPointF(12.1,13.2)"));
    gtError() << v2d;
    EXPECT_TRUE(log.contains("QVector2D(14.1, 15)"));
    gtError() << v3d;
    EXPECT_TRUE(log.contains("QVector3D(16.1, 17, 18)"));
    gtError() << v4d;
    EXPECT_TRUE(log.contains("QVector4D(19.1, 20, 21, 22)"));
}

TEST_F(Types, QWidget)
{
    QWidget const* w{};
    gtError() << w;
    EXPECT_TRUE(log.contains("QWidget(0x0)") ||
                log.contains("QObject(0x0)"));
}

