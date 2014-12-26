/***************************************************************************************************
*
* FILE:     indexdirdlg.h
*
* CREATED:  07-08-2010
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  this dialog shows the progress when indexing the files in a directory
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef INDEXDIRDLG_H
#define INDEXDIRDLG_H

#include <QDialog>

#include "database/picturedatabase.h"

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
    void reject();

protected:
    void changeEvent(QEvent *e);

private slots:
    void onCancelButtonPressed();
    void onIndexingFinished();

private:
    void readSettings();
    void writeSettings();

    Ui::IndexDirDlg *ui;
    PictureDatabase *m_database;
};

#endif // INDEXDIRDLG_H
