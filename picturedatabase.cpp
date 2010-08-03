#include "picturedatabase.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
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

bool PictureDatabase::doubleFiles()
{
    for (int i = 0; i < this->m_pictureInfo->size(); i++) {
        //QFileInfo iFile(this->m_pictureInfo->at(i)->getFile());
        for (int k = 0; k < this->m_pictureInfo->size(); k++) {
            if (i != k) {
                /*QFileInfo kFile(this->m_pictureInfo->at(k)->getFile());
                if (kFile.canonicalFilePath() == iFile.canonicalFilePath()) {
                    return true;
                }*/
                if (this->m_pictureInfo->at(i)->getFile()
                    == this->m_pictureInfo->at(k)->getFile()) {
                    return true;
                }
            }
        }
    }
    return false;
}

void PictureDatabase::removeDoubleFiles()
{
    for (int i = 0; i < this->m_pictureInfo->size(); i++) {
        //QFileInfo iFile(this->m_pictureInfo->at(i)->getFile());
        for (int k = 0; k < this->m_pictureInfo->size(); k++) {
            if (i != k) {
                /*QFileInfo kFile(this->m_pictureInfo->at(k)->getFile());
                if (kFile.canonicalFilePath() == iFile.canonicalFilePath()) {
                    this->removeEntry(k);
                    k--;
                }*/
                if (this->m_pictureInfo->at(i)->getFile()
                    == this->m_pictureInfo->at(k)->getFile()) {
                    this->removeEntry(k);
                    k--;
                }
            }
        }
    }
}

void PictureDatabase::clearPictureInfo()
{
    for (int i = 0; i < this->m_pictureInfo->size(); i++) {
        delete m_pictureInfo->value(i);
    }
    m_pictureInfo->clear();
}

void PictureDatabase::removeEntry(int index)
{
    if ((index >= 0) && (index < this->m_pictureInfo->size())) {
        delete this->m_pictureInfo->value(index);
        this->m_pictureInfo->remove(index);
    }
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
    if (identifier != FILE_ID) {
        inFile.close();
        return false;
    }
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
    this->removeDoubleFiles();
    this->m_processingWasCanceled = false;
    this->m_processRunning = true;
    this->m_processThread->processImages(this->m_pictureInfo);
    disconnect(this->m_processThread,
               SIGNAL(finished()),
               this,
               SLOT(processThreadFinished()));
    connect(this->m_processThread,
            SIGNAL(finished()),
            this,
            SLOT(processThreadFinished()));
    disconnect(this->m_processThread,
               SIGNAL(complete(float)),
               this,
               SLOT(processProgressFromThread(float)));
    connect(this->m_processThread,
            SIGNAL(complete(float)),
            this,
            SLOT(processProgressFromThread(float)));
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
    this->removeDoubleFiles();
}

void PictureDatabase::indexFile(const QString &filename)
{
    PictureInfo *newEntry = new PictureInfo;
    newEntry->setFile(QDir::toNativeSeparators(QDir::cleanPath(filename)));
    newEntry->setProcessed(false);
    newEntry->setValidFile(true);
    this->m_pictureInfo->append(newEntry);
    this->removeDoubleFiles();
}

void PictureDatabase::processThreadFinished()
{
    this->m_processRunning = false;
    emit this->processFinished(this->m_processingWasCanceled);
}

void PictureDatabase::indexThreadFinished()
{
    this->m_indexRunning = false;
    emit this->indexFinished();
}

bool PictureDatabase::isProcessingRunning()
{
    return this->m_processRunning;
}

void PictureDatabase::cancelProcessing()
{
    while (this->m_processThread->isRunning()) {
        this->m_processThread->cancel();
        this->m_processingWasCanceled = true;
    }
}

bool PictureDatabase::isIndexingRunning()
{
    return this->m_indexRunning;
}

PictureDatabase::PictureDatabase()
{
    this->m_pictureInfo = new QVector<PictureInfo*>;
    this->m_processThread = new ProcessImagesThread;
    this->m_indexThread = new IndexFilesThread;
    this->m_processRunning = false;
    this->m_indexRunning = false;
    this->m_processingWasCanceled = false;
}

PictureDatabase::~PictureDatabase()
{
    if (this->m_pictureInfo) {
        this->clearPictureInfo();
        delete this->m_pictureInfo;
    }
    if (this->m_processThread) {
        delete this->m_processThread;
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

bool PictureDatabase::allUpToDate()
{
    for (int i = 0; i < this->m_pictureInfo->size(); i++) {
        if (this->m_pictureInfo->at(i)->validFile()) {
            if (this->m_pictureInfo->at(i)->processed()) {
                QFileInfo fileInfo(this->m_pictureInfo->at(i)->getFile());
                if (fileInfo.lastModified() != this->m_pictureInfo->at(i)->lastChanged()) {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}

int PictureDatabase::filesNotUpToDate()
{
    int result = 0;
    for (int i = 0; i < this->m_pictureInfo->size(); i++) {
        if (this->m_pictureInfo->at(i)->validFile()) {
            if (this->m_pictureInfo->at(i)->processed()) {
                QFileInfo fileInfo(this->m_pictureInfo->at(i)->getFile());
                if (fileInfo.lastModified() != this->m_pictureInfo->at(i)->lastChanged()) {
                    //file changed since last analysis
                    result++;
                }
            } else {
                //file not processed yet
                result++;
            }
        } else {
            //not a valid file
            result++;
        }
    }
    return result;
}
