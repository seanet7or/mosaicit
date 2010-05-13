#ifndef MOSAICDETAILSDLG_H
#define MOSAICDETAILSDLG_H

#include <QDialog>

namespace Ui {
    class MosaicDetailsDlg;
}

class MosaicDetailsDlg : public QDialog {
    Q_OBJECT
public:
    MosaicDetailsDlg(QWidget *parent = 0);
    ~MosaicDetailsDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MosaicDetailsDlg *ui;
};

#endif // MOSAICDETAILSDLG_H
