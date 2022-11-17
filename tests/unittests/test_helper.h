/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 12.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>
#include <QDir>
#include <QUuid>

// helper for temp unittests paths
class TestHelper
{
public:
    /**
     * @brief instance
     * @return
     */
    static TestHelper& instance()
    {
        static TestHelper self;
        return self;
    }

    //! generates a new file path
    QByteArray generateFilePath(QString const& suffix = ".txt") const
    {
        QDir dir(tempPath());

        assert(dir.exists());

        return dir.absoluteFilePath(
                    QUuid::createUuid().toString() + suffix).toUtf8();
    }

    //! returns temp path
    QByteArray tempPath() const
    {
        return QDir::toNativeSeparators(QCoreApplication::applicationDirPath() +
                                        "/unittest-temp/").toUtf8();
    }

    //! removes temppath
    void reset()
    {
        QDir dir(tempPath());

        if (dir.exists() && !dir.removeRecursively())
        {
            assert(!"Error: could not remove temp dir!");
            return;
        }

        if (!dir.mkpath(dir.absolutePath()))
        {
            assert(!"Error: could not make temp dir!");
        }
    }

private:

    //! ctor -> reset temp dir on start up
    TestHelper() { reset(); }
};


#endif // TEST_HELPER_H
