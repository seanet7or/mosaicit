#ifndef PICTUREDATABASE_H
#define PICTUREDATABASE_H

#include <QVector>

#include "pictureinfo.h"
#include "processimagesthread.h"

class PictureDatabase : public QObject
{
    Q_OBJECT
public:
    PictureDatabase();
    ~PictureDatabase();
    void addDirectory(QString directory, bool subdirs);
    void processFiles();
    bool isProcessingRunning();
public slots:
    void cancelProcessing();
signals:
    void processFinished();
private slots:
    void processThreadFinished();
private:
    void addFile(QString file);

    QVector<PictureInfo*> *pictureInfo;
    ProcessImagesThread *processThread;
    bool processRunning;
};

#endif // PICTUREDATABASE_H
