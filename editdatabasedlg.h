#ifndef EDITDATABASEDLG_H
#define EDITDATABASEDLG_H

#include <QDialog>

namespace Ui {
    class EditDatabaseDlg;
}

class EditDatabaseDlg : public QDialog {
    Q_OBJECT
public:
    EditDatabaseDlg(QWidget *parent = 0);
    ~EditDatabaseDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditDatabaseDlg *ui;
};

#endif // EDITDATABASEDLG_H
