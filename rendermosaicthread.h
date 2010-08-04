#ifndef RENDERMOSAICTHREAD_H
#define RENDERMOSAICTHREAD_H

#include <QThread>
#include <QString>
#include <QWidget>

#include "picturedatabase.h"

class RenderMosaicThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderMosaicThread(QObject *parent = 0);
    void renderMosaic(PictureDatabase *database,
                      const QString &imageFile,
                      int tileWidth,
                      int tileHeight,
                      int tileCount,
                      bool cutEdges,
                      int alphaChannel,
                      const QString &outputFile,
                      QWidget *parentWindow,
                      bool minDistanceChecker,
                      int minDinstance);
    void cancel();
    bool criticalError();
    bool wasCanceled();
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

    bool m_cancelNow;
    bool m_criticalError;
    PictureDatabase *m_database;
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
};

#endif // RENDERMOSAICTHREAD_H
