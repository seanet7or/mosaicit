#include "rendermosaicthread.h"

#include <math.h>

#include <QImage>

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
    emit logText(tr("Rendering mosaic"));
    emit logText(tr("loading original image %1").arg(this->m_imageFile));
    QImage originalImage(this->m_imageFile);
    if (originalImage.isNull()) {
        emit logText(tr("Error loading the original image!"));
        return;
    }
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
    emit logText(tr("Mosaic created."));
}
