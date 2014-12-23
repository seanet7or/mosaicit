/***************************************************************************************************
*
* FILE:     processimagesthread.cpp
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

#include "processimagesthread.h"

#include <QImage>
#include <QFileInfo>

#include "debug.h"

#define log(text) Debug::log(text)

ProcessImagesThread::ProcessImagesThread()
{
    m_cancelNow = false;
    this->pictures = 0;
}

void ProcessImagesThread::cancel()
{
    m_cancelNow = true;
}

void ProcessImagesThread::processImages(QVector<PictureInfo *> *pictures)
{
    Q_ASSERT (pictures != 0);
    this->pictures = pictures;
    m_cancelNow = false;
    start();
}

void ProcessImagesThread::run()
{
    Q_ASSERT(this->pictures != 0);
    qDebug() << ("ProcessImagesThread::run called");
    for (int i = 0; i < this->pictures->size(); i++) {
        processImage(pictures->value(i));
        if (m_cancelNow) {
            qDebug() << ("ProcessImagesThread::run was canceled");
            return;
        }
        emit complete(100.f * (float)i / (float)(pictures->size() - 1));
    }
    qDebug() << ("ProcessImagesThread::run done completly");
}

bool ProcessImagesThread::processImage(PictureInfo *picture)
{
    Q_ASSERT(picture != 0);
    qDebug() << ("ProcessImagesThread::processImage called for file " +
        picture->getFile());
    QFileInfo fileInfo(picture->getFile());
    if (!fileInfo.exists()) {
        picture->setValidFile(false);
        picture->setProcessed(false);
        return false;
    }
    if ((picture->processed()) && (picture->lastChanged() == fileInfo.lastModified())) {
        //picture info is up to date
        return true;
    }
    picture->setLastChanged(fileInfo.lastModified());
    QImage *image = new QImage(picture->getFile());
    if (image->isNull()) {
        picture->setValidFile(false);
        delete image;
        return false;
    }
    picture->setValidFile(true);
    picture->setDimensions(image->width(), image->height());
    unsigned long long red = 0, green = 0, blue = 0;
    for (int i = 0; i < image->width(); i++) {
        for (int j = 0; j < image->height(); j++) {
            QRgb color = image->pixel(QPoint(i, j));
            red += qRed(color);
            green += qGreen(color);
            blue += qBlue(color);
        }
    }
    unsigned int pixels = image->width() * image->height();
    red /= pixels;
    green /= pixels;
    blue /= pixels;
    picture->setColor(red, green, blue);
    delete image;
    picture->setProcessed(true);
    qDebug() << ("ProcessImagesThread::processImage done with success");
    return true;
}
