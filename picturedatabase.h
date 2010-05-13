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
    bool isIndexingRunning();
    bool isProcessingRunning();
public slots:
    void cancelProcessing();
signals:
    void processFinished();
    void indexFinished();
private slots:
    void processThreadFinished();
    void indexThreadFinished();
private:

    QVector<PictureInfo*> *pictureInfo;
    ProcessImagesThread *processThread;
    IndexFilesThread *m_indexThread;
    bool processRunning;
    bool m_indexRunning;
};

#endif // PICTUREDATABASE_H
