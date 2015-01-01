/***************************************************************************************************
*
* FILE:     indexfilesthread.cpp
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
#include "indexfilesthread.h"
#include <QDir>
#include <QStringList>
#include <QStandardPaths>
#include <QtCore>
#include <QDesktopServices>
#include <QThread>
#include "../debug.h"

IndexFilesThread::IndexFilesThread(QStringList directories) :
    QThread()
{
    this->m_directories = directories;
    this->m_database = new PictureDatabase();
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation));
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::TempLocation));
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::CacheLocation));
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation));
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation));
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation));
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation));
    m_directoriesToExclude.append(QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation));
}

IndexFilesThread::~IndexFilesThread()
{
    if (m_database != NULL)
    {
        delete m_database;
        m_database = NULL;
    }
}

void IndexFilesThread::startIndexing()
{
    if (m_database->isOpened())
    {
        start(QThread::IdlePriority);
    }
}

void IndexFilesThread::checkFile(const QString &file)
{
    if (isInterruptionRequested())
    {
        exit(0);
        return;
    }
    QString cleanPath = QDir::toNativeSeparators(QDir::cleanPath(file));

    foreach (QString dirToExclude, m_directoriesToExclude) {
       if (dirToExclude.length()) {
           if (cleanPath.startsWith(dirToExclude)) {
               return;
           }
       }
    }

    if (!m_database->isPictureInDb(cleanPath))
    {
       // qDebug() << "Adding file" << cleanPath;
        m_database->insertNewPicture(cleanPath);
    }
    else
    {
        //qDebug() << "File" << cleanPath << "is already in DB.";
    }
    if (isInterruptionRequested())
    {
        exit(0);
        return;
    }
}

void IndexFilesThread::run()
{   
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.jpeg"; // << "*.png" << "*.bmp";
    qDebug() << "IndexFilesThread starting:" << m_directories;
    foreach (QString dir, m_directories)
    {
        if (isInterruptionRequested())
        {
            exit(0);
            return;
        }
        qDebug() << "Scanning directory" << dir;
        QDirIterator dirIterator(dir,
                                 nameFilters,
                                 QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                                 QDirIterator::Subdirectories);

        while (dirIterator.hasNext())
        {
            if (isInterruptionRequested())
            {
                exit(0);
                return;
            }
            checkFile(dirIterator.next());
        }
    }
    qDebug() << "IndexFilesThread finished.";
}
