/***************************************************************************************************
*
* FILE:     processimagesthread.h
*
* CREATED:  10-05-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  analyses the images in a database in a separate thread
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

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
    bool m_cancelNow;
};

#endif // PROCESSIMAGESTHREAD_H
