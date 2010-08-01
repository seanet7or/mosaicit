#ifndef RENDERMOSAICTHREAD_H
#define RENDERMOSAICTHREAD_H

#include <QThread>
#include <QString>

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
                      int alphaChannel);
    void cancel();

signals:
    void logText(const QString &text);
    void renderComplete(float percent);

public slots:

protected:
    void run();

private:
    bool m_cancelNow;
    PictureDatabase *m_database;
    QString m_imageFile;
    int m_tileWidth;
    int m_tileHeight;
    int m_tileCount;
    bool m_cutEdges;
    int m_alphaChannel;
};

#endif // RENDERMOSAICTHREAD_H
