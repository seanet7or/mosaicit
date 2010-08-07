/***************************************************************************************************
*
* FILE:     picturedatabase.h
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  functions to handle a picture database
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef PICTUREDATABASE_H
#define PICTUREDATABASE_H

#include <QVector>
#include <QString>

#include "pictureinfo.h"
#include "processimagesthread.h"
#include "indexfilesthread.h"

class PictureDatabase : public QObject
{
    Q_OBJECT
public:
    PictureDatabase();
    ~PictureDatabase();
    void processFiles();
    void indexFiles(QString directory, bool includeSubdirectories);
    void indexFile(const QString &filename);
    bool isIndexingRunning();
    bool isProcessingRunning();
    bool toFile(const QString &file);
    bool fromFile(const QString &file);
    void setName(const QString &name);
    QString name();
    void cancelIndexing();
    void clearPictureInfo();
    int size();
    PictureInfo *pictureAt(int index);
    void removeEntry(int index);
    bool allUpToDate();
    int filesNotUpToDate();
    bool doubleFiles();
    void removeDoubleFiles();
public slots:
    void cancelProcessing();
signals:
    void processFinished(bool wasCanceled);
    void indexFinished();
    void processProgress(float percent);
private slots:
    void processThreadFinished();
    void indexThreadFinished();
    void processProgressFromThread(float percent);
private:

    QVector<PictureInfo*> *m_pictureInfo;
    ProcessImagesThread *m_processThread;
    IndexFilesThread *m_indexThread;
    bool m_processRunning;
    bool m_indexRunning;
    bool m_processingWasCanceled;
    QString m_name;
};

#endif // PICTUREDATABASE_H
