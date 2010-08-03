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

public slots:
    void closeEvent(QCloseEvent *e);
    void reject();

protected:
    void changeEvent(QEvent *e);

private slots:
    void onFileSelected(int row);
    void onDelEntryButtonPressed();
    void onUpdateDBButtonPressed();
    void onAddDirButtonPressed();
    void onAddFileButtonPressed();
    void onCloseButtonPressed();

private:
    void updateUIElements();

    Ui::EditDatabaseDlg *ui;
    PictureDatabase *m_database;
    QString m_databaseFile;
};

#endif // EDITDATABASEDLG_H
