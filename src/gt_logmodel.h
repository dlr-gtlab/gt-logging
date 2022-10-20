/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 08.10.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTLOGMODEL_H
#define GTLOGMODEL_H

#include "gt_logging_exports.h"

#include <QAbstractItemModel>
#include <QMutex>
#include <QTimer>

#define gtLogModel (GtLogModel::instance())

/**
 * @brief The GtLogModel class
 */
class GT_LOGGING_EXPORT GtLogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        LogLevelRole = Qt::UserRole
    };

    /**
     * @brief instance
     * @return
     */
    static GtLogModel* instance();

    /**
     * @brief rowCount
     * @param parent
     * @return
     */
    int rowCount(
            const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    /**
     * @brief data
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    /**
     * @brief exportLogToFile
     * @param filename
     * @return
     */
    bool exportLogToFile(const QString& filename);

    /**
     * @brief mimeData
     * @param indexes
     * @return
     */
    QMimeData* mimeData(const QModelIndexList& indexes) const;

    /**
     * @brief Sets maximum log length (lines)
     * @param Max. number of lines
     */
    void setMaxLogLength(int val);

    /**
     * @brief removeElement - removes elemts from logmodel
     * @param index -  index corresponds to the parent from
     * which the new rows are removed (see QAbstractItemModel::beginRemoveRows)
     * @param first - index of first element to be removed
     * @param last - index of last elment to be removed
     */
    void removeElement(QModelIndex index, int first, int last);

    /**
     * @brief removeElement - removes elemts from logmodel
     * @param indexList -  index corresponds to the parent from
     * which the new rows are removed (see QAbstractItemModel::beginRemoveRows)
     * @param first - index of first element to be removed
     * @param last - index of last elment to be removed
     */
    void removeElementList(QModelIndexList indexList, int first, int last);

public slots:
    void onMessage(const QString& msg ,int level);

    void clearLog();

protected:
    /**
     * @brief GtLogModel
     */
    explicit GtLogModel(QObject* parent);

private:
    struct Entry
    {
        int m_level;
        QString m_msg;
    };

    QVector<Entry> m_entries;

    QVector<Entry> m_tmpEntries;

    QMutex m_mutex;

    QTimer m_timer;

    bool m_tmpClearLog;

    int m_maxEntries;

    void insertMessage(const QString& msg ,int level);

    void execClear();

private slots:
    void insertQueue();

};

#endif // GTLOGMODEL_H
