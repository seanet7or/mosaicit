/***************************************************************************************************
*
* FILE:     rendermosaicthread.h
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
#ifndef RENDERMOSAICTHREAD_H
#define RENDERMOSAICTHREAD_H
#include <QThread>
#include <QString>
#include <QWidget>
#include "picturelibrary.h"

class RenderMosaicThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderMosaicThread(QObject *parent = 0);
    void renderMosaic(const QString &imageFile,
                      int tileWidth,
                      int tileHeight,
                      int tileCount,
                      bool cutEdges,
                      int alphaChannel,
                      const QString &outputFile,
                      QWidget *parentWindow,
                      bool minDistanceChecker,
                      int minDinstance,
                      bool maxTileRepeatChecker,
                      int maxTileRepeatCount);
    bool criticalError();
    QString outputFile();

signals:
    void logText(const QString &text);
    void renderComplete(float percent);

public slots:

protected:
    void run();

private:
    float smallestDistance(int **tileMap,
                           int mapWidth,
                           int mapHeight,
                           int currentTile,
                           int xPos,
                           int yPos);
    int tileCountInMap(int **tileMap, int mapWidth, int mapHeight, int tileToSearch);
    QImage imageMatchingTileSize(const QString &file);
    QImage imageCroppedToDesiredAspectRatio(QImage image);

    QString m_tileCacheDir;
    bool m_criticalError;
    PictureLibrary *m_pictureLibrary;
    QString m_imageFile;
    int m_tileWidth;
    int m_tileHeight;
    int m_tileCount;
    bool m_cutEdges;
    int m_alphaChannel;
    QString m_outputFile;
    QWidget *m_parentWindow;
    bool m_minDistanceChecker;
    int m_minDistance;
    bool m_maxTileRepeatChecker;
    int m_maxTileRepeatCount;
};

#endif // RENDERMOSAICTHREAD_H
