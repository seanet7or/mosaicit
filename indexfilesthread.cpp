/***************************************************************************************************
*
* FILE:     indexfilesthread.cpp
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

#include "indexfilesthread.h"

#include <QDir>
#include <QStringList>

#include "debug.h"


IndexFilesThread::IndexFilesThread(QObject *parent) :
    QThread(parent)
{
    this->m_canceled = false;
}

void IndexFilesThread::indexDirectory(QVector<PictureInfo *> *pictures,
                                      QString directory,
                                      bool subdirs)
{
    Q_ASSERT (pictures != 0);
    Q_ASSERT (directory != QString());
    this->m_canceled = false;
    this->m_directory = directory;
    this->m_pictures = pictures;
    this->m_includeSubdirectories = subdirs;
    start();
}

void IndexFilesThread::addDirectory(QString directory, bool subdirs)
{
    qDebug() << ("IndexFilesThread::addDirectory called");
    if (this->m_canceled) {
        qDebug() << ("  canceled!");
        return;
    }
    qDebug() << ("  for directory " + directory);
    QDir rootDir(directory);
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";
    QStringList files = rootDir.entryList(filters, QDir::Files);
    foreach (QString file, files) {
        if (this->m_canceled) {
            qDebug() << ("  canceled!");
            return;
        }
        qDebug() << ("  adding file " + directory + "/" + file);
        addFile(directory + "/" + file);
    }
    if (subdirs) {
        QStringList dirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (QString dir, dirs) {
            if (this->m_canceled) {
                qDebug() << ("  canceled!");
                return;
            }
            qDebug() << ("  calling for dir " + dir);
            addDirectory(directory + "/" + dir, subdirs);
        }
    }
    qDebug() << ("IndexFilesThread::addDirectory finished");
}

void IndexFilesThread::addFile(QString file)
{
	qDebug() << "Adding file" << file;
    PictureInfo *newEntry = new PictureInfo;
    newEntry->setFile(QDir::toNativeSeparators(QDir::cleanPath(file)));
    newEntry->setProcessed(false);
    newEntry->setValidFile(true);
    this->m_pictures->append(newEntry);
}

void IndexFilesThread::run()
{
    this->addDirectory(this->m_directory,
                       this->m_includeSubdirectories);
}

void IndexFilesThread::cancel()
{
    this->m_canceled = true;
}
