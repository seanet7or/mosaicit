#ifndef CREATEMOSAICDLG_H
#define CREATEMOSAICDLG_H

#include <QDialog>

namespace Ui {
    class CreateMosaicDlg;
}

class CreateMosaicDlg : public QDialog {
    Q_OBJECT
public:
    CreateMosaicDlg(QWidget *parent = 0);
    ~CreateMosaicDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CreateMosaicDlg *ui;

private slots:
    void selectDBBnPressed();
    void selectImageBnPressed();
    void imageChanged();
};

#endif // CREATEMOSAICDLG_H
