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

protected:
    void changeEvent(QEvent *e);

private slots:
    void tileWidthChanged();
    void tileHeightChanged();
    void aspectRatioChanged();
    void updateResultLabel();

private:
    Ui::MosaicDetailsDlg *ui;
    QString m_imageFile;
    QImage m_image;
};

#endif // MOSAICDETAILSDLG_H
