/***************************************************************************************************
*
* FILE:     mosaicdetailsdlg.h
*
* CREATED:  13-05-2010
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  dialog to set detailed options for a mosaic
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef MOSAICDETAILSDLG_H
#define MOSAICDETAILSDLG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class MosaicDetailsDlg;
}

class MosaicDetailsDlg : public QDialog {
    Q_OBJECT
public:
    MosaicDetailsDlg(QWidget *parent, const QString &imageFile);
    ~MosaicDetailsDlg();
    bool exitedCorrectly();
    int tileWidth() const;
    int tileHeight() const;
    int tileCount() const;
    bool cutEdges();
    int alphaChannel();
    bool minDistanceChecker();
    int minDistance();
    bool repeatTilesMaxChecker();
    int repeatTilesMaxCount();

protected:
    void changeEvent(QEvent *e);

private slots:
    void tileWidthChanged(int);
    void totalTilesCountChanged(int);
    void aspectRatioChanged();
    void updateResultLabels();
    void renderButtonPressed();
    void cancelButtonPressed();
    void closeEvent(QCloseEvent *e);

private:
    void updateValuesFromForms();
    void writeSettings();
    void readSettings();

    Ui::MosaicDetailsDlg *ui;
    QString m_imageFile;
    QImage m_image;
    bool m_cutEdges;
    int m_alphaChannel;
    bool m_canceled;
    bool m_minDistanceChecker;
    int m_minDistance;
    bool m_repeatTilesChecker;
    int m_repeatTilesMax;
};

#endif // MOSAICDETAILSDLG_H
