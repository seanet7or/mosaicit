#include "picturedatabase.h"

#include <QDir>

#include "debug.h"

#define log(text)   //Debug::log(text)

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
    this->pictureInfo->append(newEntry);
}

void PictureDatabase::processFiles()
{
    connect(this->processThread,
            SIGNAL(finished()),
            this,
            SLOT(processThreadFinished()));
    this->processRunning = true;
    this->processThread->processImages(this->pictureInfo);
}

void PictureDatabase::processThreadFinished()
{
    this->processRunning = false;
    emit this->processFinished();
}

bool PictureDatabase::isProcessingRunning()
{
    return this->processRunning;
}

void PictureDatabase::cancelProcessing()
{
    this->processThread->cancel();
}

PictureDatabase::PictureDatabase()
{
    this->pictureInfo = new QVector<PictureInfo*>;
    this->processThread = new ProcessImagesThread;
    this->processRunning = false;
}

PictureDatabase::~PictureDatabase()
{
    if (this->pictureInfo) {
        delete this->pictureInfo;
    }
    if (this->processThread) {
        delete this->processThread;
    }
}
