/***************************************************************************************************
*
* FILE:     rendermosaicdlg.h
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  shows the progress when rendering a mosaic
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef RENDERMOSAICDLG_H
#define RENDERMOSAICDLG_H

#include <QDialog>
#include <QString>

#include "picturedatabase.h"
#include "rendermosaicthread.h"

namespace Ui {
    class RenderMosaicDlg;
}

class RenderMosaicDlg : public QDialog {
    Q_OBJECT
public:
    RenderMosaicDlg(QWidget *parent,
                    const QString &imageFile,
                    int tileWidth,
                    int tileHeight,
                    int tileCount,
                    bool cutEdges,
                    int alphaChannel,
                    const QString &outputFile,
                    bool minDistChecker,
                    int minDist,
                    bool maxTileRepeatChecker,
                    int maxTileRepeatCount);
    ~RenderMosaicDlg();

public slots:
    void logText(const QString &text);
    void closeEvent(QCloseEvent *e);
    void reject();

private slots:
    void renderComplete(float percent);
    void cancelBnPressed();
    void renderThreadFinished();

protected:
    void changeEvent(QEvent *e);

private:
    void writeSettings();
    void readSettings();

    Ui::RenderMosaicDlg *ui;
    RenderMosaicThread *m_renderThread;
    QString m_outputFile;
};

#endif // RENDERMOSAICDLG_H
