#ifndef PROCESSIMAGESTHREAD_H
#define PROCESSIMAGESTHREAD_H

#include <QThread>
#include <QVector>

#include "pictureinfo.h"

class ProcessImagesThread : public QThread
{
    Q_OBJECT
public:
    ProcessImagesThread();
    void processImages(QVector<PictureInfo*> *pictures);

signals:
    void complete(float percent);

public slots:
    void cancel();

protected:
    void run();

private:
    bool processImage(PictureInfo *picture);
    QVector<PictureInfo*> *pictures;
    bool cancelNow;
};

#endif // PROCESSIMAGESTHREAD_H
