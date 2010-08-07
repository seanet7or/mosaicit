/***************************************************************************************************
*
* FILE:     aboutdlg.h
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  Header file for the about dialog
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>

#include "appsettings.h"

namespace Ui {
    class AboutDlg;
}

class AboutDlg : public QDialog {
    Q_OBJECT
public:
    AboutDlg(QWidget *parent = 0);
    ~AboutDlg();

protected:
    void changeEvent(QEvent *e);

private slots:
    void onOkButtonPressed();

private:
    void writeSettings();
    void readSettings();
    Ui::AboutDlg *ui;
};

#endif // ABOUTDLG_H
