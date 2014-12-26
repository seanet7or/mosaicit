/***************************************************************************************************
*
* FILE:     indexfilesthread.h
*
* CREATED:  13-05-2010
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  seperate thread to index all files in a directory
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/
#ifndef INDEXFILESTHREAD_H
#define INDEXFILESTHREAD_H
#include <QThread>
#include <QVector>
#include <QString>
#include <QStringList>
#include "../database/picturedatabase.h"

class IndexFilesThread : public QThread
{
    Q_OBJECT
public:
    explicit IndexFilesThread(
            QStringList directories);
    ~IndexFilesThread();

    void startIndexing();

signals:

public slots:

protected:
    void run();

private:
    void checkFile(const QString &file);

private:
    PictureDatabase *m_database;
    QStringList m_directories;
};

#endif // INDEXFILESTHREAD_H
