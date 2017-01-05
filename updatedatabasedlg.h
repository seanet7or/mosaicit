/***************************************************************************************************
*
* FILE:     updatedatabasedlg.h
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  dialog that shows the process when updating a database
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

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

public slots:
    void closeEvent(QCloseEvent *e);
    void reject();

protected:
    void changeEvent(QEvent *e);

private slots:
    void cancelButtonPressed();
    void processDone(float percent);
    void processComplete(bool wasCanceled);

private:
    void readSettings();
    void writeSettings();

    Ui::UpdateDatabaseDlg *ui;
    PictureDatabase *m_database;
};

#endif // UPDATEDATABASEDLG_H
