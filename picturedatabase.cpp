#include "picturedatabase.h"

#include <QDir>
#include <QFile>
#include <QDataStream>

#include "debug.h"

#define log(text)   Debug::log(text)

#define FILE_ID     0x4D494442

bool PictureDatabase::toFile(const QString &file)
{
    log("PictureDatabase::toFile called");
    quint32 identifier = FILE_ID;
    quint32 majorVersion = 1;
    quint32 minorVersion = 0;
    quint32 entryCount = this->pictureInfo->size();
    QFile outFile(file);
    if (!outFile.open(QIODevice::WriteOnly)) {
        log("  unable to open file");
        return false;
    }
    QDataStream out(&outFile);
    out << identifier;
    out << majorVersion;
    out << minorVersion;
    out << this->m_name;
    out << entryCount;
    for (int i = 0; i < this->pictureInfo->size(); i++) {
        this->pictureInfo->at(i)->toStream(out);
    }
    outFile.close();
    log("PictureDatabase::toFile done with success");
    return true;
}

void PictureDatabase::cancelIndexing()
{
    while (this->m_indexThread->isRunning()) {
        this->m_indexThread->cancel();
    }
}

void PictureDatabase::setName(const QString &name)
{
    this->m_name = name;
}

QString PictureDatabase::name()
{
    return this->m_name;
}

void PictureDatabase::processFiles()
{
    connect(this->processThread,
            SIGNAL(finished()),
            this,
            SLOT(processThreadFinished()));
    connect(this->processThread,
            SIGNAL(complete(float)),
            this,
            SLOT(processProgressFromThread(float)));
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
    while (this->processThread->isRunning()) {
        this->processThread->cancel();
    }
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

void PictureDatabase::processProgressFromThread(float percent)
{
    log("PictureDatabase::processProgressFromThread called, "
        + QString::number(percent));
    emit this->processProgress(percent);
}
