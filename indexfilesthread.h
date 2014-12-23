/***************************************************************************************************
*
* FILE:     indexfilesthread.h
*
* CREATED:  13-05-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
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

#include "pictureinfo.h"

class IndexFilesThread : public QThread
{
    Q_OBJECT
public:
    explicit IndexFilesThread(QObject *parent = 0);
    void indexDirectory(QVector<PictureInfo*> *pictures,
                        QString directory,
                        bool subdirs);

signals:

public slots:
    void cancel();

protected:
    void run();

private:
    void addDirectory(QString directory, bool subdirs);
    void addFile(QString file);

    bool m_canceled;
    QString m_directory;
    QVector<PictureInfo*> *m_pictures;
    bool m_includeSubdirectories;
};

#endif // INDEXFILESTHREAD_H
