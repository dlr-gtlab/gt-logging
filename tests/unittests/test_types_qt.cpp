/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 17.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#include "test_log_helper.h"
#include "gt_logging/stl_bindings.h"
#include "gt_logging/qt_bindings.h"

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QModelIndex>

#include <QWidget>
#include <QJsonValue>

// test fixture
class TypesQt : public LogHelperTest {};

// testing some std types to check for ambigiuous overloads
TEST_F(TypesQt, std_vector)
{
    gtInfo() << std::vector<double>{14, 43, 15};
    EXPECT_TRUE(log.contains("(14, 43, 15)"));
}

TEST_F(TypesQt, std_list)
{
    // this doesnt work because we defined the operator<< only for gt::log::Stream
    gtInfo() << std::list<MyStruct>{MyStruct{12}};
    EXPECT_TRUE(log.contains("(MyStruct(12))"));

    gtInfo() << std::list<float>{0.1f, 0.2f, 0.3f};
    EXPECT_TRUE(log.contains("(0.1, 0.2, 0.3)"));
}

TEST_F(TypesQt, std_map)
{
    std::map<float, QString> map;
    map.insert({41.2f, "ABC"});

    gtInfo() << map;
    EXPECT_TRUE(log.contains(R"(map{(41.2, "ABC")})"));
}

TEST_F(TypesQt, std_multimap)
{
    std::multimap<float, QString> map;
    map.insert({41.1f, "ABC"});
    map.insert({41.2f, "Test"});

    gtInfo() << map;
    EXPECT_TRUE(log.contains("multimap{("));
    EXPECT_TRUE(log.contains(R"((41.1, "ABC"))"));
    EXPECT_TRUE(log.contains(R"((41.2, "Test"))"));
    EXPECT_TRUE(log.contains("}"));
}

TEST_F(TypesQt, std_pair)
{
    std::pair<QString, int> pair{"test", 42};
    gtInfo() << pair;
    EXPECT_TRUE(log.contains(R"(("test", 42))"));
}

TEST_F(TypesQt, QStringList)
{
    gtInfo() << QStringList{"Hallo", "Welt"};
    EXPECT_TRUE(log.contains("(Hallo, Welt)"));
}

TEST_F(TypesQt, QList)
{
    gtInfo() << QList<int>{14, 43, 15};
    EXPECT_TRUE(log.contains("(14, 43, 15)"));

    gtInfo() << QList<MyStruct>{MyStruct{12}, MyStruct{13}, MyStruct{14}};
    EXPECT_TRUE(log.contains("(MyStruct(12), MyStruct(13), MyStruct(14))"));
}

TEST_F(TypesQt, QVector)
{
    gtInfo() << QVector<char>{'A', 'B', 'C'};
    EXPECT_TRUE(log.contains("(A, B, C)"));

    gtInfo() << QVector<MyStruct>{MyStruct{12}, MyStruct{13}, MyStruct{14}};
    EXPECT_TRUE(log.contains("(MyStruct(12), MyStruct(13), MyStruct(14))"));
}

TEST_F(TypesQt, QHash)
{
    QHash<QString, int> hash;
    hash.insert("a", 1);
    hash.insert("b", 2);

    gtInfo() << hash;

    // note, the order of args is random
    EXPECT_TRUE(log.contains("QHash("));
    EXPECT_TRUE(log.contains("(a, 1)"));
    EXPECT_TRUE(log.contains("(b, 2)"));
}

TEST_F(TypesQt, QMap)
{
    QMap<QString, int> map;
    map.insert("a", 11);
    map.insert("b", 22);

    gtInfo() << map;
    // note, the order of args is random
    EXPECT_TRUE(log.contains("QMap("));
    EXPECT_TRUE(log.contains("(a, 11)"));
    EXPECT_TRUE(log.contains("(b, 22)"));
}

TEST_F(TypesQt, QSet)
{
    QSet<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(33);

    gtInfo() << set;
    // note, the order of args is random
    EXPECT_TRUE(log.contains("QSet("));
    EXPECT_TRUE(log.contains("1"));
    EXPECT_TRUE(log.contains("2"));
    EXPECT_TRUE(log.contains("33"));
}

TEST_F(TypesQt, QPair)
{
    QPair<QString, int> pair{"test", 42};
    gtInfo() << pair;
    EXPECT_TRUE(log.contains("QPair(test,42)"));
}

TEST_F(TypesQt, QSharedPointer)
{
    QSharedPointer<int> ptr{new int{42}};

    gtInfo() << ptr;
    EXPECT_TRUE(log.contains("QSharedPointer("));
    EXPECT_TRUE(log.contains(QString::number((size_t)ptr.data(), 16)));
}
TEST_F(TypesQt, QContiguousCache)
{
    QContiguousCache<int> cache;

    gtInfo() << cache;
    EXPECT_TRUE(log.contains("QContiguousCache()"));
}

TEST_F(TypesQt, QVariant)
{
    gtInfo() << QVariant("A_Variant");
    EXPECT_TRUE(log.contains("QVariant(QString, A_Variant)"));

    gtInfo() << QVariant(42);
    EXPECT_TRUE(log.contains("QVariant(int, 42)"));
}

TEST_F(TypesQt, QObject)
{
    MyQObject obj;
    obj.setObjectName("MyObjectName");

    gtInfo() << &obj;

    EXPECT_TRUE(log.contains("MyObjectName"));
    EXPECT_TRUE(log.contains(QString::number((size_t)&obj, 16), Qt::CaseInsensitive));
}

TEST_F(TypesQt, MyStructObject)
{
    MyStruct obj{42};
    gtInfo() << obj;
    gtInfo() << &obj;

    EXPECT_TRUE(log.contains("MyStruct(42)"));
    EXPECT_TRUE(log.contains(QString::number((size_t)&obj, 16), Qt::CaseInsensitive));
}

TEST_F(TypesQt, QEnum)
{
    auto e = MyQObject::FirstEntry;

    gtInfo() << e;
    EXPECT_TRUE(log.contains("MyQObject::FirstEntry"));
};

TEST_F(TypesQt, QFlags)
{
    MyQObject::Flags f = {MyQObject::A | MyQObject::C};

    gtInfo() << f;
    EXPECT_TRUE(log.contains("QFlags(0x1|0x4)"));
};

TEST_F(TypesQt, POD_chars)
{
    QChar qc = 'Q';

    gtInfo() << qc;
    EXPECT_TRUE(log.contains(qc));
}

TEST_F(TypesQt, Strings)
{
    QString qstr = "MyFancyString";
    QByteArray qba = "MyPreciousBytes";
    QLatin1String qlt1{"MyLatinString"};
    std::string s = "MyStdString";

    // also test for const
    gtInfo() << qstr << qAsConst(qstr);
    EXPECT_TRUE(log.contains(qstr));
    gtInfo() << qba << qAsConst(qba);
    EXPECT_TRUE(log.contains(qba));
    gtInfo() << qlt1 << qAsConst(qlt1);
    EXPECT_TRUE(log.contains(qlt1));
    gtInfo() << s << qAsConst(s);
    EXPECT_TRUE(log.contains(s.c_str()));
    gtInfo() << QObject::tr("Test");
    EXPECT_TRUE(log.contains("Test"));
}

TEST_F(TypesQt, QStringRef)
{
    QString qstr = "MyFancyString";
    QStringRef qstrr = &qstr;

    gtInfo() << qstrr;
    EXPECT_TRUE(log.contains(qstr));
}

TEST_F(TypesQt, StringViews)
{
    QTextStreamFunction qstrf{};
    gtInfo() << qstrf;
    EXPECT_TRUE(log.isEmpty());

    QString qstr = "MyFancyString";
    QStringView qstrv{qstr};

    auto myStrFunc = [](QTextStream& stream) -> QTextStream&{
        return stream << "Test";
    };
    QTextStreamFunction qstrf2{myStrFunc};

    gtInfo() << qstrv;
    EXPECT_TRUE(log.contains(qstr));
    gtInfo() << qstrf2;
    EXPECT_TRUE(log.contains("Test"));
}

TEST_F(TypesQt, QPoints)
{
    QModelIndex idx;
    QPoint pt{10, 11};
    QPointF ptf{12.1, 13.2};
    QVector2D v2d{14.1f, 15};
    QVector3D v3d{16.1f, 17, 18};
    QVector4D v4d{19.1f, 20, 21, 22};

    gtInfo() << idx;
    EXPECT_TRUE(log.contains("QModelIndex(-1,-1,0x0,QObject(0x0))"));
    gtInfo() << pt;
    EXPECT_TRUE(log.contains("QPoint(10,11)"));
    gtInfo() << ptf;
    EXPECT_TRUE(log.contains("QPointF(12.1,13.2)"));
    gtInfo() << v2d;
    EXPECT_TRUE(log.contains("QVector2D(14.1, 15)"));
    gtInfo() << v3d;
    EXPECT_TRUE(log.contains("QVector3D(16.1, 17, 18)"));
    gtInfo() << v4d;
    EXPECT_TRUE(log.contains("QVector4D(19.1, 20, 21, 22)"));
}

TEST_F(TypesQt, QWidget)
{
    QWidget const* w{};
    gtInfo() << w;
    EXPECT_TRUE(log.contains("QWidget(0x0)") ||
                log.contains("QObject(0x0)"));
}

