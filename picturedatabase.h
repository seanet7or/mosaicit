/***************************************************************************************************
*
* FILE:     picturedatabase.h
*
* CREATED:  2014-12-23
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  Database keeping all known pictures
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright by Benjamin Caspari
*
***************************************************************************************************/
#ifndef PICTUREDATABASE_H
#define PICTUREDATABASE_H
#include <QString>
#include <QSqlQuery>
#include <QMutex>
#include <QDateTime>

#define PICTURE_STATE_NOTPROCESSED              1
#define PICTURE_STATE_CORRUPT                   2
#define PICTURE_STATE_PROCESSED                 3

class PictureDatabase
{
public:
    PictureDatabase();
    ~PictureDatabase();

    static bool openDb(QString dbFile = QString::null);

    bool isPictureInDb(const QString &path);
    bool insertNewPicture(const QString &path);
    QStringList picturesNotProcessed();
    bool setPictureProperties(
            const QString &path,
            QDateTime mtime,
            int state,
            int width,
            int height,
            quint32 rgb);

    bool isOpened() const;

private:
    static QString databaseFile();
    static bool execSQLQuery(const QString &query,
                      bool ignoreErrors = false);
    static bool execSQLQuery(QSqlQuery *query);

private:
    static bool m_open;
    QSqlQuery *m_isPictureInDbQuery;
    QSqlQuery *m_insertNewPictureQuery;
    QSqlQuery *m_picturesNotProcessedQuery;
    QSqlQuery *m_setPicturePropertiesQuery;
    static QMutex m_lock;
};

#endif // PICTUREDATABASE_H
