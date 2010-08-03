#ifndef INDEXDIRDLG_H
#define INDEXDIRDLG_H

#include <QDialog>

#include "picturedatabase.h"

namespace Ui {
    class IndexDirDlg;
}

class IndexDirDlg : public QDialog {
    Q_OBJECT
public:
    IndexDirDlg(QWidget *parent, PictureDatabase *database, const QString &newDir);
    ~IndexDirDlg();

public slots:
    void closeEvent(QCloseEvent *e);

protected:
    void changeEvent(QEvent *e);

private slots:
    void onCancelButtonPressed();
    void onIndexingFinished();

private:
    Ui::IndexDirDlg *ui;
    PictureDatabase *m_database;
};

#endif // INDEXDIRDLG_H
