/***************************************************************************************************
*
* FILE:     newdatabasedlg.h
*
* CREATED:  13-05-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  dialog to create a new database file
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef NEWDATABASEDLG_H
#define NEWDATABASEDLG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class NewDatabaseDlg;
}

class NewDatabaseDlg : public QDialog {
    Q_OBJECT
public:
    NewDatabaseDlg(QWidget *parent = 0);
    ~NewDatabaseDlg();
    bool exitedCorrectly();
    QString name();
    QString directory();
    bool includeSubdirectories();

public slots:
    void closeEvent(QCloseEvent *e);

protected:
    void changeEvent(QEvent *e);

private slots:
    void buildButtonPressed();
    void selectDirButtonPressed();
    void cancelButtonPressed();
    void selectFileButtonPressed();

private:
    void readSettings();
    void writeSettings();

    Ui::NewDatabaseDlg *ui;

    QString m_name;
    QString m_directory;
    bool m_includeSubdirs;
    bool m_canceled;
};

#endif // NEWDATABASEDLG_H
