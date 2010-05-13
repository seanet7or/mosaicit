#include "indexfilesthread.h"

#include <QDir>
#include <QStringList>

#include "debug.h"
#define log(text) Debug::log(text)

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
    log("IndexFilesThread::addDirectory called");
    if (this->m_canceled) {
        log("  canceled!");
        return;
    }
    log("  for directory " + directory);
    QDir rootDir(directory);
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";
    QStringList files = rootDir.entryList(filters, QDir::Files);
    foreach (QString file, files) {
        if (this->m_canceled) {
            log("  canceled!");
            return;
        }
        log("  adding file " + directory + "/" + file);
        addFile(directory + "/" + file);
    }
    if (subdirs) {
        QStringList dirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (QString dir, dirs) {
            if (this->m_canceled) {
                log("  canceled!");
                return;
            }
            log("  calling for dir " + dir);
            addDirectory(directory + "/" + dir, subdirs);
        }
    }
    log("IndexFilesThread::addDirectory finished");
}

void IndexFilesThread::addFile(QString file)
{
    PictureInfo *newEntry = new PictureInfo;
    newEntry->setFile(file);
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
