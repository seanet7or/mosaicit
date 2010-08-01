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
    quint32 entryCount = this->m_pictureInfo->size();
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
    for (int i = 0; i < this->m_pictureInfo->size(); i++) {
        this->m_pictureInfo->at(i)->toStream(out);
    }
    outFile.close();
    log("PictureDatabase::toFile done with success");
    return true;
}

void PictureDatabase::clearPictureInfo()
{
    for (int i = 0; i < this->m_pictureInfo->size(); i++) {
        delete m_pictureInfo->value(i);
    }
    m_pictureInfo->clear();
}

bool PictureDatabase::fromFile(const QString &file)
{
    this->clearPictureInfo();
    QFile inFile(file);
    if (!inFile.open(QIODevice::ReadOnly)) {
        return false;
    }
    QDataStream in(&inFile);
    quint32 identifier = 0;
    quint32 majorVersion;
    quint32 minorVersion;
    QString name;
    quint32 entryCount;
    in >> identifier;
    in >> majorVersion;
    in >> minorVersion;
    in >> name;
    in >> entryCount;
    for (unsigned int i = 0; i < entryCount; i++) {
        PictureInfo *newEntry = new PictureInfo;
        newEntry->fromStream(in);
        this->m_pictureInfo->append(newEntry);
    }
    inFile.close();
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
    this->processThread->processImages(this->m_pictureInfo);
}

void PictureDatabase::indexFiles(QString directory,
                                 bool includeSubdirectories)
{
    connect(this->m_indexThread,
            SIGNAL(finished()),
            this,
            SLOT(indexThreadFinished()));
    this->m_indexRunning = true;
    this->m_indexThread->indexDirectory(this->m_pictureInfo,
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
    this->m_pictureInfo = new QVector<PictureInfo*>;
    this->processThread = new ProcessImagesThread;
    this->m_indexThread = new IndexFilesThread;
    this->processRunning = false;
    this->m_indexRunning = false;
}

PictureDatabase::~PictureDatabase()
{
    if (this->m_pictureInfo) {
        this->clearPictureInfo();
        delete this->m_pictureInfo;
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

int PictureDatabase::size()
{
    return this->m_pictureInfo->size();
}

PictureInfo *PictureDatabase::pictureAt(int index)
{
    if ((index >= 0) && (index < this->m_pictureInfo->size())) {
        return this->m_pictureInfo->value(index);
    }
    return 0;
}
