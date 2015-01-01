/***************************************************************************************************
*
* FILE:     picturedatabase.cpp
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
#include "picturedatabase.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "../settings.h"

#define TABLE_PICTURES                          "pictures"
#define COLUMN_PICTURES_PATH                    "path"
#define COLUMN_PICTURES_MTIME                   "mtime"
#define COLUMN_PICTURES_STATE                   "state"
#define COLUMN_PICTURES_WIDTH                   "width"
#define COLUMN_PICTURES_HEIGHT                  "height"
#define COLUMN_PICTURES_RGB                     "rgb"

bool PictureDatabase::m_open = false;
QMutex PictureDatabase::m_lock;

PictureDatabase::PictureDatabase()
{
    m_isPictureInDbQuery = new QSqlQuery(
                "SELECT EXISTS(SELECT 1 FROM " TABLE_PICTURES
                " WHERE " COLUMN_PICTURES_PATH " = ? "\
                "LIMIT 1)");

    m_insertNewPictureQuery = new QSqlQuery(
                "INSERT INTO " TABLE_PICTURES
                "(" COLUMN_PICTURES_PATH ") "\
                "VALUES (?)");

    m_picturesNotProcessedQuery = new QSqlQuery(
                "SELECT " COLUMN_PICTURES_PATH " FROM " TABLE_PICTURES
                " WHERE " COLUMN_PICTURES_STATE " = " + QString::number(PICTURE_STATE_NOTPROCESSED));

    m_processedPicturesQuery = new QSqlQuery(
                "SELECT " COLUMN_PICTURES_PATH ", " COLUMN_PICTURES_RGB ", " COLUMN_PICTURES_WIDTH ", " COLUMN_PICTURES_HEIGHT " FROM " TABLE_PICTURES
                " WHERE " COLUMN_PICTURES_STATE " = " + QString::number(PICTURE_STATE_PROCESSED));

    QString setPicturePropertiesCmd = "UPDATE " TABLE_PICTURES
            " SET " COLUMN_PICTURES_MTIME " = ?, "
            COLUMN_PICTURES_STATE " = ?, "
            COLUMN_PICTURES_WIDTH " = ?, "
            COLUMN_PICTURES_HEIGHT " = ?, "
            COLUMN_PICTURES_RGB " = ? "
            " WHERE " COLUMN_PICTURES_PATH " = ?";
    m_setPicturePropertiesQuery = new QSqlQuery(setPicturePropertiesCmd);

    m_deletePictureQuery = new QSqlQuery(
                "DELETE FROM " TABLE_PICTURES " WHERE " COLUMN_PICTURES_PATH " = ?");
}

bool PictureDatabase::deletePicture(const QString &path)
{
    m_lock.lock();
    m_deletePictureQuery->addBindValue(path);
    bool res = execSQLQuery(m_deletePictureQuery);
    m_lock.unlock();
    return res;
}

QVector<PictureInfo*> PictureDatabase::picturesProcessed()
{
    m_lock.lock();
    QVector<PictureInfo*> result;
    if (execSQLQuery(m_processedPicturesQuery))
    {
        result = QVector<PictureInfo*>();
        while (m_processedPicturesQuery->next())
        {
            PictureInfo *currentRow = new PictureInfo(m_processedPicturesQuery->value(0).toString(),
                                                      m_processedPicturesQuery->value(2).toInt(),
                                                      m_processedPicturesQuery->value(3).toInt(),
                                                      m_processedPicturesQuery->value(1).toUInt());
            result.append(currentRow);
        }
    }
    else
    {
        result = QVector<PictureInfo*>();
    }
    m_lock.unlock();
    return result;
}

bool PictureDatabase::openDb(QString dbFile)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (dbFile.isNull())
    {
        dbFile = databaseFile();
    }

    qDebug() << "Using database file " << dbFile;
    db.setDatabaseName(dbFile);
    m_open = db.open();
    if (!m_open)
    {
        qCritical() << "Opening the database failed.";
        return false;
    }

    if (!db.tables().contains(TABLE_PICTURES))
    {
        execSQLQuery("CREATE TABLE IF NOT EXISTS " TABLE_PICTURES\
                "(" COLUMN_PICTURES_PATH " TEXT PRIMARY KEY NOT NULL ON CONFLICT IGNORE, "\
                COLUMN_PICTURES_MTIME " INT DEFAULT 0, "\
                COLUMN_PICTURES_STATE " INT DEFAULT " + QString::number(PICTURE_STATE_NOTPROCESSED) + ", "\
                COLUMN_PICTURES_WIDTH " INT DEFAULT 0, "\
                COLUMN_PICTURES_HEIGHT " INT DEFAULT 0, "\
                COLUMN_PICTURES_RGB " INT DEFAULT 0, "\
                "UNIQUE (" COLUMN_PICTURES_PATH ") ON CONFLICT IGNORE)");
    }

    execSQLQuery("PRAGMA foreign_keys = OFF");
    execSQLQuery("PRAGMA synchronous = OFF");
    execSQLQuery("PRAGMA journal_mode = MEMORY");

    return true;
}

bool PictureDatabase::setPictureProperties(
        const QString &path,
        QDateTime mtime,
        int state,
        int width,
        int height,
        quint32 rgb)
{
    m_lock.lock();
    m_setPicturePropertiesQuery->addBindValue(mtime.currentMSecsSinceEpoch());
    m_setPicturePropertiesQuery->addBindValue(state);
    m_setPicturePropertiesQuery->addBindValue(width);
    m_setPicturePropertiesQuery->addBindValue(height);
    m_setPicturePropertiesQuery->addBindValue(rgb);
    m_setPicturePropertiesQuery->addBindValue(path);
    bool res = execSQLQuery(m_setPicturePropertiesQuery);
    m_lock.unlock();
    return res;
}

QStringList PictureDatabase::picturesNotProcessed()
{
    m_lock.lock();
    QStringList result;
    if (execSQLQuery(m_picturesNotProcessedQuery))
    {
        while (m_picturesNotProcessedQuery->next())
        {
            result.append(m_picturesNotProcessedQuery->value(0).toString());
        }
    }
    m_lock.unlock();
    return result;
}

bool PictureDatabase::insertNewPicture(const QString &path)
{
    m_lock.lock();
    m_insertNewPictureQuery->addBindValue(path);
    bool res = execSQLQuery(m_insertNewPictureQuery);
    m_lock.unlock();
    return res;
}

bool PictureDatabase::isPictureInDb(const QString &path)
{
    m_lock.lock();
    bool res = false;
    m_isPictureInDbQuery->addBindValue(path);
    if (execSQLQuery(m_isPictureInDbQuery) && m_isPictureInDbQuery->next())
    {
        bool ok;
        int intRes = m_isPictureInDbQuery->value(0).toInt(&ok);
        if (ok)
        {
            res = intRes > 0;
        }
    }
    m_lock.unlock();
    return res;
}

PictureDatabase::~PictureDatabase()
{
    if (m_isPictureInDbQuery != NULL)
    {
        delete m_isPictureInDbQuery;
        m_isPictureInDbQuery = NULL;
    }
    if (m_insertNewPictureQuery != NULL)
    {
        delete m_insertNewPictureQuery;
        m_insertNewPictureQuery = NULL;
    }
}

QString PictureDatabase::databaseFile()
{
    return Settings::configDirectory() + "/pictures.db";
}

bool PictureDatabase::execSQLQuery(const QString &query,
                            bool ignoreErrors)
{
#ifdef DEBUG_DATABASE
    qDebug() << "Executing SQL query: " << query;
#endif //#ifdef DEBUG_DATABASE
    QSqlQuery sqlQuery(query);

    bool res = sqlQuery.exec();    
    if (!res && !ignoreErrors)
    {
        qWarning() << "SQL query " << query
                   << " failed: " << sqlQuery.lastError().text();
    }
    return res;
}

bool PictureDatabase::execSQLQuery(QSqlQuery *query)
{
#ifdef DEBUG_DATABASE
    qDebug() << "Executing SQL query: " << query;
#endif //#ifdef DEBUG_DATABASE
    bool res = query->exec();
    if (!res)
    {
        qWarning() << "SQL query " << query
                   << " failed: " << query->lastError().text();
    }
    return res;
}

bool PictureDatabase::isOpened() const
{
    return m_open;
}
