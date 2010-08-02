#include "processimagesthread.h"

#include <QImage>
#include <QFileInfo>

#include "debug.h"

#define log(text) Debug::log(text)

ProcessImagesThread::ProcessImagesThread()
{
    cancelNow = false;
    this->pictures = 0;
}

void ProcessImagesThread::cancel()
{
    cancelNow = true;
}

void ProcessImagesThread::processImages(QVector<PictureInfo *> *pictures)
{
    Q_ASSERT (pictures != 0);
    this->pictures = pictures;
    start();
}

void ProcessImagesThread::run()
{
    Q_ASSERT(this->pictures != 0);
    log("ProcessImagesThread::run called");
    for (int i = 0; i < this->pictures->size(); i++) {
        processImage(pictures->value(i));
        if (cancelNow) {
            log("ProcessImagesThread::run was canceled");
            return;
        }
        emit complete(100.f * (float)i / (float)(pictures->size() - 1));
    }
    log("ProcessImagesThread::run done completly");
}

bool ProcessImagesThread::processImage(PictureInfo *picture)
{
    Q_ASSERT(picture != 0);
    log("ProcessImagesThread::processImage called for file " +
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
    log("ProcessImagesThread::processImage done with success");
    return true;
}
