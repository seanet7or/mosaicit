#ifndef EDITDATABASEDLG_H
#define EDITDATABASEDLG_H

#include <QDialog>

#include "picturedatabase.h"

namespace Ui {
    class EditDatabaseDlg;
}

class EditDatabaseDlg : public QDialog {
    Q_OBJECT
public:
    EditDatabaseDlg(QWidget *parent, const QString &databaseFile);
    ~EditDatabaseDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditDatabaseDlg *ui;
    PictureDatabase *m_database;
};

#endif // EDITDATABASEDLG_H
