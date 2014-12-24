#include "databaseworkmanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QMap>
#include <QDebug>

DatabaseWorkManager::DatabaseWorkManager()
{
    this->m_database = new PictureDatabase();    
}

DatabaseWorkManager::~DatabaseWorkManager()
{
    foreach (IndexFilesThread *t, m_filesystemIndexers)
    {
        t->exit();
        delete t;
    }
    m_filesystemIndexers.clear();

    foreach (PictureAnalyzerThread *t, m_pictureAnalyzers)
    {
        t->exit();
        delete t;
    }
    m_pictureAnalyzers.clear();
}

void DatabaseWorkManager::requestCancel()
{
    requestInterruption();
    foreach (IndexFilesThread *t, m_filesystemIndexers)
    {
        if (t->isRunning())
        {
            t->requestInterruption();
        }
    }

    foreach (IndexFilesThread *t, m_filesystemIndexers)
    {
        if (t->isRunning())
        {
            if (!t->wait(30 * 1000))
            {
                qWarning() << "IndexFilesThread cancel timeout, shutting it down.";
                t->quit();
            }
        }
    }
}

void DatabaseWorkManager::startWork()
{
    start();
}

void DatabaseWorkManager::run()
{
    startFilesystemIndexers();

    int analyzerWorkerThreads = QThread::idealThreadCount();
    if (analyzerWorkerThreads < 1)
    {
        analyzerWorkerThreads = 1;
    }
    else
    {
        qDebug() << "Using" << analyzerWorkerThreads << "threads to analyze pictures.";
    }

    while (!isInterruptionRequested())
    {
        QStringList picturesNotProcessed = m_database->picturesNotProcessed();

        while (picturesNotProcessed.length() > 0)
        {
            if (isInterruptionRequested())
            {
                exit(0);
                return;
            }

            qDebug() << picturesNotProcessed.length() << "pictures to be analyzed.";
            while (m_pictureAnalyzers.length() >= analyzerWorkerThreads)
            {
                for (int i = 0; i < m_pictureAnalyzers.length(); i++)
                {
                    PictureAnalyzerThread* t = m_pictureAnalyzers.at(i);
                    if (t->isFinished())
                    {
                        m_pictureAnalyzers.removeAt(i);
                        delete t;
                        break;
                    }
                }
            }

            QString picture = picturesNotProcessed.at(0);
            PictureAnalyzerThread *analyzerThread = new PictureAnalyzerThread();
            analyzerThread->startAnalyzer(picture);
            picturesNotProcessed.removeAt(0);
            m_pictureAnalyzers.append(analyzerThread);
        }
    }

    if (isInterruptionRequested())
    {
        exit(0);
        return;
    }
}

void DatabaseWorkManager::startFilesystemIndexers()
{
    QStringList dirs;
    dirs.append(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation));
    dirs.append(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation));
    dirs.append(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation));
    dirs.append(QStandardPaths::standardLocations(QStandardPaths::DownloadLocation));
    dirs.append(QStandardPaths::standardLocations(QStandardPaths::HomeLocation));
    dirs.append(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation));
    dirs.append(QStandardPaths::standardLocations(QStandardPaths::MusicLocation));
    foreach (QFileInfo drive, QDir::drives())
    {
        dirs.append(drive.path());
    }

    QMap<QChar, QStringList> dirsByDrive;
    foreach (QString dir, dirs)
    {
         QChar drive = '?';
         if ((dir.length() >= 2) && (dir.at(1) == ':'))
         {
            drive = dir.at(0);
         }
         if (!dirsByDrive.contains(drive))
         {
             dirsByDrive.insert(drive, QStringList());
         }
         dirsByDrive[drive].append(dir);
    }

    foreach (QChar drive, dirsByDrive.keys())
    {
        IndexFilesThread *newWorker = new IndexFilesThread(dirsByDrive[drive]);
        newWorker->startIndexing();
        m_filesystemIndexers.append(newWorker);
    }
}
