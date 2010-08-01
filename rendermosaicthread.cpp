#include "rendermosaicthread.h"

#include <math.h>

#include <QImage>
#include <QPainter>

RenderMosaicThread::RenderMosaicThread(QObject *parent) :
        QThread(parent)
{
    m_cancelNow = false;
}

void RenderMosaicThread::renderMosaic(PictureDatabase *database,
                                      const QString &imageFile,
                                      int tileWidth,
                                      int tileHeight,
                                      int tileCount,
                                      bool cutEdges,
                                      int alphaChannel)
{
    m_cancelNow = false;
    this->m_database = database;
    this->m_imageFile = imageFile;
    this->m_tileWidth = tileWidth;
    this->m_tileHeight = tileHeight;
    this->m_tileCount = tileCount;
    this->m_cutEdges = cutEdges;
    this->m_alphaChannel = alphaChannel;
    start();
}

void RenderMosaicThread::cancel()
{
    this->m_cancelNow = true;
}

void RenderMosaicThread::run()
{
    //this function will build the new mosaic picture according to the infos
    //passed to the corresponding class
    emit logText(tr("Rendering mosaic"));

    //first, load the original image into memory
    emit logText(tr("loading original image %1").arg(this->m_imageFile));
    QImage originalImage(this->m_imageFile);
    if (originalImage.isNull()) {
        emit logText(tr("Error loading the original image!"));
        return;
    }

    //now, compute the exact amount of tiles in vertical (x) and horizontal
    //(y) direction
    /*
      tx * ty = tileCount => tx = tileCount / ty
       and
      imageW / imageH = (tx * tileWidth) / (ty * tileHeight)
      => ty * tileHeight * imageW /imageH = (tx * tileWidth)
      => ty * tileHeight/tileWidth * imageW/imageH = tileCount / ty
       with tileHeight/tileWidth * imageW/imageH = aspectFactor
      => ty * ty = tileCount / aspectFactor
      => ty = sqrt(tileCount / aspectFactor)
      */
    double aspectFactor = ((double)this->m_tileHeight
                           / (double)this->m_tileWidth)
                          * ((double)originalImage.width()
                             / (double)originalImage.height());
    int tilesY = (int)sqrt((double)this->m_tileCount / aspectFactor);
    int tilesX = this->m_tileCount / tilesY;
    emit logText(tr("The mosaic will be built of %1 x %2 tiles").arg(
            QString::number(tilesX), QString::number(tilesY)));

    //resize the original file to get the color values for the
    //unique tiles
    QImage tilesImages = originalImage.scaled(tilesX, tilesY,
                                              Qt::IgnoreAspectRatio,
                                              Qt::SmoothTransformation
                                              );
    if (tilesImages.isNull()) {
        emit logText(tr("Error scaling the original picture"));
        return;
    }

    //reserve image for the mosaic
    QImage mosaic(tilesX * this->m_tileWidth, tilesY * this->m_tileHeight,
                  QImage::Format_ARGB32);
    if (mosaic.isNull()) {
        emit logText(tr("Error reserving memory for the mosaic!"));
        return;
    }

    //loop through all tiles
    for (int i = 0; i < tilesX; i++) {
        for (int j = 0; j < tilesY; j++) {

            //look for best matching tile for current pixel
            QRgb thisPixel = tilesImages.pixel(i, j);
            int minDiff = 1000;
            int nearestIndex = -1;
            //loop through all tiles and look for the one with the best matching
            //color
            for (int k = 0; k < this->m_database->size(); k++) {
                if (this->m_database->pictureAt(k)->processed()) {
                    int diff = 0;
                    diff += qAbs(qRed(thisPixel)
                                 - this->m_database->pictureAt(k)->getRed());
                    diff += qAbs(qGreen(thisPixel)
                                 - this->m_database->pictureAt(k)->getGreen());
                    diff += qAbs(qBlue(thisPixel)
                                 - this->m_database->pictureAt(k)->getBlue());
                    if (diff < minDiff) {
                        minDiff = diff;
                        nearestIndex = k;
                    }
                }
            }
            //found matching tile
            if (nearestIndex != -1) {
                emit logText(tr("found matching tile!"));
                emit logText(tr("filename is %1").arg(
                        this->m_database->pictureAt(nearestIndex)->getFile()));
                //load tile image
                QImage tileImage(
                        this->m_database->pictureAt(nearestIndex)->getFile());
                tileImage = tileImage.scaled(this->m_tileWidth,
                                             this->m_tileHeight,
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation);
                if (tileImage.isNull()) {
                    emit logText(tr("Error loading and scaling tile!"));
                    return;
                }
                //copy into mosaic
                QPainter mosaicPainter(&mosaic);
                mosaicPainter.drawImage(i * this->m_tileWidth,
                                        j * this->m_tileHeight,
                                        tileImage);
            } else {
                emit logText("no matching tile found!");
            }

        }
    }

    mosaic.save("/media/data/m.jpg");
    emit logText(tr("Mosaic created."));
}
