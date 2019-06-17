/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 08.10.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QCoreApplication>
#include <QFile>
#include <QMimeData>
#include <QUrl>

#include "gt_logmodel.h"
#include "gt_logging.h"
#include "QsLogDest.h"

GtLogModel::GtLogModel(QObject* parent) : QAbstractListModel(parent),
    m_tmpClearLog(false),
    m_maxEntries(2000)
{
    QsLogging::Logger& logger = QsLogging::Logger::instance();

    QsLogging::DestinationPtr widgetDestination(
                QsLogging::DestinationFactory::MakeFunctorDestination(
                    this, SLOT(onMessage(QString,int))));

    logger.addDestination(widgetDestination);

    connect(&m_timer, SIGNAL(timeout()), SLOT(insertQueue()));
}

QMimeData*
GtLogModel::mimeData(const QModelIndexList& indexes) const
{
    QString str;

    foreach (const QModelIndex& index, indexes)
    {
        if (index.row() < 0 || index.row() >= m_entries.count())
        {
            continue;
        }

        const int lvl = m_entries[index.row()].m_level;

        QsLogging::Level level = QsLogging::Logger::levelFromInt(lvl);

        str = str + QsLogging::Logger::levelToString(level) + "\t" +
              m_entries[index.row()].m_msg + "\r\n";
    }

    QMimeData* mimeData = new QMimeData;
    mimeData->setText(str);

    return mimeData;
}

void
GtLogModel::setMaxLogLength(int val)
{
    if (val <= 10)
    {
        return;
    }

    m_maxEntries = val;
}

int
GtLogModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_entries.count();
}

QVariant
GtLogModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_entries.count())
    {
        return QVariant();
    }

    const Entry& entry = m_entries[index.row()];

    switch (role)
    {
    case Qt::DisplayRole:
        return entry.m_msg;
    case GtLogModel::LogLevelRole:
        return entry.m_level;
    }

    return QVariant();
}

bool
GtLogModel::exportLogToFile(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QFile::WriteOnly))
    {
        gtError() << "could not open file! (" << filename << ")";
        return false;
    }

    QTextStream out(&file);

    foreach (const Entry& entry, m_entries)
    {
        QsLogging::Level level = QsLogging::Logger::levelFromInt(entry.m_level);

        out << QsLogging::Logger::levelToString(level) << "\t"
            << entry.m_msg << "\r\n";
    }

    return true;
}

void
GtLogModel::onMessage(const QString& msg, int level)
{
    m_mutex.lock();

    if (m_timer.isActive())
    {
        m_tmpEntries << Entry({ level, msg });
    }
    else
    {
        insertMessage(msg, level);
        m_timer.start(500);
    }

    m_mutex.unlock();
}

void
GtLogModel::execClear()
{
    if (m_tmpClearLog)
    {
        beginResetModel();
        m_entries.clear();
        endResetModel();
        m_tmpClearLog = false;
    }
}

void
GtLogModel::clearLog()
{
    m_tmpClearLog = true;

    if (m_timer.isActive())
    {

        return;
    }

    execClear();
}

void
GtLogModel::insertMessage(const QString& msg, int level)
{
    if (m_entries.size() + 1 >= m_maxEntries)
    {
        beginRemoveRows(QModelIndex(), 0, 0);
        m_entries.removeFirst();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), m_entries.size(), m_entries.size());
    m_entries << Entry({ level, msg });
    endInsertRows();
}

void
GtLogModel::removeElement(QModelIndex index, int first, int last)
{
    beginRemoveRows(index, first, last);
    for (int i = 0; i <= (last - first); ++i)
    {
        m_entries.removeAt(first);
    }
    endRemoveRows();

}

void
GtLogModel::removeElementList(QModelIndexList indexList, int first, int last)
{
    int size = indexList.size();

    beginRemoveRows(indexList.first().parent(), first, last);

    for (int i = 0; i < size; ++i)
    {
        m_entries.removeAt(indexList.at(size - 1 - i).row());
    }

    endRemoveRows();

}

void
GtLogModel::insertQueue()
{
    m_timer.stop();

    execClear();

    if (m_tmpEntries.isEmpty())
    {
        return;
    }

    int newRows = m_tmpEntries.size();

    // check whether size of new entries exceeds the maximum allowed entries
    if (newRows > m_maxEntries)
    {
        for (int i = 0; i < (newRows - m_maxEntries); i++)
        {
            m_tmpEntries.removeFirst();
        }

        newRows = m_tmpEntries.size();
    }

    int delta = (m_entries.size() + m_tmpEntries.size()) - m_maxEntries;

    if (delta > m_maxEntries)
    {
        for (int i = 0; i < (delta - m_maxEntries); i++)
        {
            m_tmpEntries.removeFirst();
        }
    }

    newRows = m_tmpEntries.size() - 1;
    delta = (m_entries.size() + m_tmpEntries.size()) - m_maxEntries;

    if (delta > 0)
    {
        beginRemoveRows(QModelIndex(), 0, delta - 1);
        for (int i = 0; i < delta; i++)
        {
            m_entries.removeFirst();
        }
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), m_entries.size(),
                    m_entries.size() + newRows);
    foreach (const Entry& entry, m_tmpEntries)
    {
        m_entries << entry;
    }
    endInsertRows();

    m_tmpEntries.clear();
}

GtLogModel*
GtLogModel::instance()
{
    static GtLogModel* retval = 0;
    if (retval == 0)
    {
        retval = new GtLogModel(qApp);
    }
    return retval;
}
