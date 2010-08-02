#ifndef UPDATEDATABASEDLG_H
#define UPDATEDATABASEDLG_H

#include <QDialog>

#include "picturedatabase.h"

namespace Ui {
    class UpdateDatabaseDlg;
}

class UpdateDatabaseDlg : public QDialog {
    Q_OBJECT
public:
    UpdateDatabaseDlg(QWidget *parent, PictureDatabase *database);
    ~UpdateDatabaseDlg();

protected:
    void changeEvent(QEvent *e);

private slots:
    void cancelButtonPressed();
    void processDone(float percent);
    void processComplete(bool wasCanceled);

private:
    Ui::UpdateDatabaseDlg *ui;
    PictureDatabase *m_database;
};

#endif // UPDATEDATABASEDLG_H
