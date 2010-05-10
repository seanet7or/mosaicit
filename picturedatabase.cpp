#include "picturedatabase.h"

#include <QDir>

#include "debug.h"

void PictureDatabase::addDirectory(QString directory, bool subdirs)
{
    log("PictureDatabaes::addDirectory called");
    log("  for directory " + directory);
    QDir rootDir(directory);
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";
    QStringList files = rootDir.entryList(filters, QDir::Files);
    foreach (QString file, files) {
        log("  adding file " + directory + "/" + file);
        addFile(directory + "/" + file);
    }
    if (subdirs) {
        QStringList dirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (QString dir, dirs) {
            log("  calling for dir " + dir);
            addDirectory(directory + "/" + dir, subdirs);
        }
    }
    log("PictureDatabase::addDirectory finished");
}

void PictureDatabase::addFile(QString file)
{
    PictureInfo *newEntry = new PictureInfo;
    newEntry->setFile(file);
    this->pictureInfo.append(newEntry);
}

PictureDatabase::PictureDatabase()
{
}
