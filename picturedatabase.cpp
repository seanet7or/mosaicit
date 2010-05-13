#include "picturedatabase.h"

#include <QDir>

#include "debug.h"

#define log(text)   //Debug::log(text)

void PictureDatabase::processFiles()
{
    connect(this->processThread,
            SIGNAL(finished()),
            this,
            SLOT(processThreadFinished()));
    this->processRunning = true;
    this->processThread->processImages(this->pictureInfo);
}

void PictureDatabase::indexFiles(QString directory,
                                 bool includeSubdirectories)
{
    connect(this->m_indexThread,
            SIGNAL(finished()),
            this,
            SLOT(indexThreadFinished()));
    this->m_indexRunning = true;
    this->m_indexThread->indexDirectory(this->pictureInfo,
                                        directory,
                                        includeSubdirectories);
}

void PictureDatabase::processThreadFinished()
{
    this->processRunning = false;
    emit this->processFinished();
}

void PictureDatabase::indexThreadFinished()
{
    this->m_indexRunning = false;
    emit this->indexFinished();
}

bool PictureDatabase::isProcessingRunning()
{
    return this->processRunning;
}

void PictureDatabase::cancelProcessing()
{
    this->processThread->cancel();
}

bool PictureDatabase::isIndexingRunning()
{
    return this->m_indexRunning;
}

PictureDatabase::PictureDatabase()
{
    this->pictureInfo = new QVector<PictureInfo*>;
    this->processThread = new ProcessImagesThread;
    this->m_indexThread = new IndexFilesThread;
    this->processRunning = false;
    this->m_indexRunning = false;
}

PictureDatabase::~PictureDatabase()
{
    if (this->pictureInfo) {
        delete this->pictureInfo;
    }
    if (this->processThread) {
        delete this->processThread;
    }
    if (this->m_indexThread) {
        delete this->m_indexThread;
    }
}
