/***************************************************************************************************
*
* FILE:     rendermosaicthread.cpp
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  allows to render the mosaic in a seperate tread
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/
#include "rendermosaicthread.h"
#include <math.h>

#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QDebug>

#include "settings.h"

RenderMosaicThread::RenderMosaicThread(QObject *parent) :
    QThread(parent)
{
    m_criticalError = false;
}

void RenderMosaicThread::renderMosaic(const QString &imageFile,
                                      int tileWidth,
                                      int tileHeight,
                                      int tileCount,
                                      bool cutEdges,
                                      int alphaChannel,
                                      const QString &outputFile,
                                      QWidget *parentWindow,
                                      bool minDistanceChecker,
                                      int minDistance,
                                      bool maxTileRepeatChecker,
                                      int maxTileRepeatCount)
{
    this->m_criticalError = false;
    this->m_pictureLibrary = new PictureLibrary();
    this->m_imageFile = imageFile;
    this->m_tileWidth = tileWidth;
    this->m_tileHeight = tileHeight;
    this->m_tileCount = tileCount;
    this->m_cutEdges = cutEdges;
    this->m_alphaChannel = alphaChannel;
    this->m_outputFile = outputFile;
    this->m_parentWindow = parentWindow;
    this->m_minDistanceChecker = minDistanceChecker;
    this->m_minDistance = minDistance;
    this->m_maxTileRepeatCount = maxTileRepeatCount;
    this->m_maxTileRepeatChecker = maxTileRepeatChecker;

    m_tileCacheDir = Settings::configDirectory();
    m_tileCacheDir += "/" + QString::number(m_tileWidth) + "x" + QString::number(m_tileHeight);
    if (m_cutEdges) {
        m_tileCacheDir += "cropped";
    }
    QDir dir(m_tileCacheDir);
    if (!dir.mkpath(m_tileCacheDir))
    {
        qWarning() << "Failed to create directory "
            << QDir::toNativeSeparators(m_tileCacheDir);
    }

    start();
}

float RenderMosaicThread::smallestDistance(int**tileMap,
                                           int mapWidth,
                                           int mapHeight,
                                           int currentTile,
                                           int xPos,
                                           int yPos)
{
    bool found = false;
    float distance = -1.f;
    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            if (tileMap[i][j] == currentTile) {
                if ((xPos != i) || (yPos != j)) {
                    //now calculate distance to second occurance of tile
                    float currentDistance = sqrt((xPos - i) * (xPos - i) + (yPos - j) * (yPos - j));
                    if (!found) {
                        found = true;
                        distance = currentDistance;
                    }
                    if (currentDistance < distance) {
                        distance = currentDistance;
                    }
                }
            }
        }
    }
    return distance;
}

void RenderMosaicThread::run()
{
    //this function will build the new mosaic picture according to the infos
    //passed to the corresponding class
    emit renderComplete(0.f);
    emit logText(tr("Rendering the mosaic..."));

    if (isInterruptionRequested()) {
        emit logText(tr("Rendering was canceled - no mosaic was saved!"));
        return;
    }

    //first, load the original image into memory
    emit logText(tr("loading original image %1.").arg(this->m_imageFile));
    QImage originalImage(this->m_imageFile);
    if (originalImage.isNull()) {
        emit logText(tr("Error loading the original image!"));
        this->m_criticalError = true;
        return;
    }

    if (isInterruptionRequested()) {
        emit logText(tr("Rendering was canceled - no mosaic was saved!"));
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
    double aspectFactor = ((double)this->m_tileHeight / (double)this->m_tileWidth)
            * ((double)originalImage.width() / (double)originalImage.height());
    int tilesY = qRound(sqrt(((double)this->m_tileCount) / aspectFactor));
    int tilesX = this->m_tileCount / tilesY;
    emit logText(tr("The mosaic will be built of %1 x %2 tiles.").arg(
                     QString::number(tilesX), QString::number(tilesY)));

    //resize the original file to get the color values for the
    //unique tiles
    QImage tilesImages = originalImage.scaled(tilesX, tilesY,
                                              Qt::IgnoreAspectRatio,
                                              Qt::SmoothTransformation
                                              );
    if (tilesImages.isNull()) {
        emit logText(tr("Error scaling the original picture!"));
        this->m_criticalError = true;
        return;
    }

    if (isInterruptionRequested()) {
        emit logText(tr("Rendering was canceled - no mosaic was saved!"));
        return;
    }

    //reserve memory for tiles map
    int **tilesMap = new int*[tilesX];
    if (!tilesMap) {
        emit logText(tr("Error reserving memory for the tiles map!"));
        this->m_criticalError = true;
        return;
    }
    for (int i = 0; i < tilesX; i++) {
        tilesMap[i] = new int[tilesY];
        if (!tilesMap[i]) {
            emit logText(tr("Error reserving memory for tiles map!"));
            this->m_criticalError = true;
            return;
        }
    }
    for (int i = 0; i < tilesX; i++) {
        for (int j = 0; j < tilesY; j++) {
            tilesMap[i][j] = -1;
        }
    }

    //reserve image for the mosaic
    QImage mosaic(tilesX * this->m_tileWidth,
                  tilesY * this->m_tileHeight,
                  QImage::Format_ARGB32_Premultiplied);
    if (mosaic.isNull()) {
        emit logText(tr("Error reserving memory for the mosaic image!"));
        this->m_criticalError = true;
        return;
    }
    QPainter mosaicPainter(&mosaic);

    emit renderComplete(1.f);

    if (isInterruptionRequested()) {
        emit logText(tr("Rendering was canceled - no mosaic was saved!"));
        return;
    }

    //loop through all tiles
    int tilesDone = 0;
    for (int i = 0; i < tilesX; i++) {
        for (int j = 0; j < tilesY; j++) {

            if (isInterruptionRequested()) {
                emit logText(tr("Rendering was canceled - no mosaic was saved!"));
                return;
            }

            //look for best matching tile for current pixel
            QRgb thisPixel = tilesImages.pixel(i, j);
            int minDiff = 1000;
            int nearestIndex = -1;
            //loop through all tiles and look for the one with the best matching
            //color
            for (int k = 0; k < this->m_pictureLibrary->size(); k++) {
                int diff = 0;
                diff += qAbs(qRed(thisPixel)
                             - this->m_pictureLibrary->pictureAt(k)->getRed());
                diff += qAbs(qGreen(thisPixel)
                             - this->m_pictureLibrary->pictureAt(k)->getGreen());
                diff += qAbs(qBlue(thisPixel)
                             - this->m_pictureLibrary->pictureAt(k)->getBlue());
                if (diff < minDiff) {
                    bool tileIsOk = true;
                    //potentially found a matching tile, but check for min distance condition
                    if (this->m_minDistanceChecker) {
                        float distance = this->smallestDistance(tilesMap,
                                                                tilesX,
                                                                tilesY,
                                                                k,
                                                                i,
                                                                j);
                        if (distance >= 0.f) {
                            if (distance < (float)this->m_minDistance) {
                                tileIsOk = false;
                                //emit logText(tr("distance too small to equal tile!"));
                            }
                        }
                    }
                    //check for max tile repeat count condition
                    if (this->m_maxTileRepeatChecker) {
                        int tileInMap = this->tileCountInMap(tilesMap, tilesX, tilesY, k);
                        if (tileInMap >= this->m_maxTileRepeatCount) {
                            tileIsOk = false;
                        }
                    }
                    if (tileIsOk) {
                        minDiff = diff;
                        nearestIndex = k;
                    }
                }

            }

            //found matching tile
            if (nearestIndex != -1) {
                tilesMap[i][j] = nearestIndex;
                //emit logText(tr("Found matching tile: Filename is %1").arg(
                //                 this->m_pictureLibrary->pictureAt(nearestIndex)->getPath()));
                QImage tileImage = imageMatchingTileSize(this->m_pictureLibrary->pictureAt(nearestIndex)->getPath());
                if (tileImage.isNull()) {
                    emit logText(tr("Error loading the image %1 in tile size!").arg(
                                     this->m_pictureLibrary->pictureAt(nearestIndex)->getPath()));
                    this->m_criticalError = true;
                    return;
                }
                //copy into mosaic
                mosaicPainter.drawImage(i * this->m_tileWidth,
                                        j * this->m_tileHeight,
                                        tileImage);
            } else {
                emit logText(tr("Error - no matching tile was found!"));
                if (this->m_maxTileRepeatChecker) {
                    emit logText(tr("You have selected a maximum repeat rate for equal tiles. Propably there are not enough images in the database to fullfill this option. Choose a higher value or uncheck the option."));
                }
                if (this->m_minDistanceChecker) {
                    emit logText(tr("You have selected a minimum distance between two equal tiles. Propably there are not enough images in the database to fullfill this option. Choose a lower value or uncheck the option."));
                }
                emit logText(tr("Choosing a image database with more entries might also remove the problem."));
                this->m_criticalError = true;
                return;
            }
            tilesDone++;
            emit renderComplete(98.f*(float)tilesDone/(float)(tilesX*tilesY) + 1.f);
        }
    }

    if (isInterruptionRequested()) {
        emit logText(tr("Rendering was canceled - no mosaic was saved!"));
        return;
    }

    //blend in alpha channel now
    if (this->m_alphaChannel > 0) {
        QRect srcRect(0, 0, originalImage.width(), originalImage.height());
        QRect destRect(0, 0, mosaic.width(), mosaic.height());
        mosaicPainter.setOpacity((float)this->m_alphaChannel / 100.f);
        mosaicPainter.drawImage(destRect,
                                originalImage,
                                srcRect);
    }

    if (isInterruptionRequested()) {
        emit logText(tr("Rendering was canceled - no mosaic was saved!"));
        return;
    }

    if (!mosaic.save(this->m_outputFile)) {
        emit logText(tr("Error saving the built mosaic in the file %1!").arg(this->m_outputFile));
        bool tryAgain = true;
        while (tryAgain) {
            if (QMessageBox::question(this->m_parentWindow,
                                      tr("Error saving mosaic"),
                                      tr("The mosaic could not be saved as \"%1\"! Do you want to select another file to write to?").arg(
                                          QDir::toNativeSeparators(
                                              QDir::cleanPath(
                                                  this->m_outputFile))),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                this->m_outputFile =
                        QFileDialog::getSaveFileName(this->m_parentWindow,
                                                     tr("Select mosaic output file"),
                                                     QDir::toNativeSeparators(
                                                         QDir::cleanPath(
                                                             QDir::homePath()
                                                             + "/mosaic.jpg")),
                                                     tr("Images (*.jpg)"));
                if (mosaic.save(this->m_outputFile)) {
                    tryAgain = false;
                }
            } else {
                tryAgain = false;
                this->m_criticalError = true;
                return;
            }
        }
    }

    //free tile map
    for (int i = 0; i < tilesX; i++) {
        delete tilesMap[i];
    }
    delete tilesMap;

    emit renderComplete(100.f);
    emit logText(tr("The mosaic was created."));
}

QImage RenderMosaicThread::imageMatchingTileSize(const QString &file)
{
    QString extension = file.split('.').last();
    if (!extension.length()) {
        extension = "jpg";
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(file.toUtf8());
    QString cacheFile = m_tileCacheDir + "/" + hash.result().toBase64().replace('=', "").replace('/', '-') + "." + extension;
    QImage imageFromCache(cacheFile);
    if (!imageFromCache.isNull()) {
        //emit logText(tr("Loaded tile for %1 from cache.").arg(file));
        return imageFromCache;
    }

    //load tile image
    QImage tileImage(file);
    if (tileImage.isNull()) {
        emit logText(tr("Error loading the tile image %1!").arg(file));
        return tileImage;
    }

    //do we have to cut the edges?
    if (this->m_cutEdges) {
        tileImage = imageCroppedToDesiredAspectRatio(tileImage);
    }

    tileImage = tileImage.scaled(this->m_tileWidth,
                            this->m_tileHeight,
                            Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation);

    if (!tileImage.save(cacheFile)) {
        qWarning() << "Error saving cached image as" << cacheFile;
    };
    //emit logText(tr("Saved tile cache for %1.").arg(file));

    return tileImage;
}

QImage RenderMosaicThread::imageCroppedToDesiredAspectRatio(QImage image)
{
    //if tile aspect ratio doesn't fit the tile aspect ratio given
    //for the mosaic
    float currentTileAspectRatio = ((float)image.width()/(float)image.height());
    float desiredTileAspectRatio = ((float)m_tileWidth/(float)m_tileHeight);

    if (!qFuzzyCompare(currentTileAspectRatio, desiredTileAspectRatio)) {
        //emit logText("Cutting images edges.");
        if (currentTileAspectRatio > desiredTileAspectRatio) {
            // example: tileWidth = 100
            // tileHeight = 100
            // tileImage.width() = 2000
            // tileImage.height() = 1000
            //  => currentTileAspectRatio = 2.0
            //  => generalTileAspectRatio = 1.0
            //  => newCurrentTileWith = (1000 / 100) * 100 = 1000
            int newCurrentTileWidth = ((float)image.height() / (float)m_tileHeight) * m_tileWidth;
            return image.copy(
                        (image.width()-newCurrentTileWidth) / 2,
                        0,
                        newCurrentTileWidth,
                        image.height());
        } else {
            // example: tileWidth = 100
            // tileHeight = 100
            // tileImage.width() = 10
            // tileImage.height() = 20
            //  => currentTileAspectRatio = 0.5
            //  => generalTileAspectRatio = 1.0
            //  => newCurrentTileHeight = (10 / 100) * 100 = 10
            int newCurrentTileHeight = ((float)image.width()  / (float)m_tileWidth) * m_tileHeight;
            //emit logText(tr("Cutting top and bottom, new height will be %1.").arg(QString::number(newCurrentTileHeight)));
            return image.copy(
                        0,
                        (image.height() - newCurrentTileHeight) / 2,
                        image.width(),
                        newCurrentTileHeight);
        }
    } else {
        return image;
    }
}

bool RenderMosaicThread::criticalError()
{
    return this->m_criticalError;
}

QString RenderMosaicThread::outputFile()
{
    return this->m_outputFile;
}

int RenderMosaicThread::tileCountInMap(int**tileMap, int mapWidth, int mapHeight, int tileToSearch)
{
    int result = 0;
    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            if (tileMap[i][j] == tileToSearch) {
                result++;
            }
        }
    }
    return result;
}
