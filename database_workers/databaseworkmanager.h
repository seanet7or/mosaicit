#ifndef DATABASEWORKMANAGER_H
#define DATABASEWORKMANAGER_H
#include <QThread>
#include "indexfilesthread.h"
#include "pictureanalyzerthread.h"

class DatabaseWorkManager : QThread
{
public:
    DatabaseWorkManager();
    ~DatabaseWorkManager();

    void startWork();
    void requestCancel();

protected:
    void run();

private:
    void startFilesystemIndexers();

private:
    QList<IndexFilesThread*> m_filesystemIndexers;
    QList<PictureAnalyzerThread*> m_pictureAnalyzers;
    PictureDatabase *m_database;
};

#endif // DATABASEWORKMANAGER_H
