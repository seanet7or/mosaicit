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
    bool exitedCorrectly();
    int tileWidth();
    int tileHeight();
    int tileCount();
    bool cutEdges();
    int alphaChannel();
    ~MosaicDetailsDlg();

protected:
    void changeEvent(QEvent *e);

private slots:
    void tileWidthChanged();
    void tileHeightChanged();
    void aspectRatioChanged();
    void updateResultLabel();
    void renderButtonPressed();
    void cancelButtonPressed();

private:
    Ui::MosaicDetailsDlg *ui;
    QString m_imageFile;
    QImage m_image;
    int m_tileWidth;
    int m_tileHeight;
    int m_tileCount;
    bool m_cutEdges;
    int m_alphaChannel;
    bool m_canceled;
};

#endif // MOSAICDETAILSDLG_H
