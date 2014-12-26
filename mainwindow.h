/***************************************************************************************************
*
* FILE:     mainwindow.h
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  the applications main window
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "appsettings.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void newMosaicBnClicked();
    void editDatabaseBnClicked();
    void aboutBnClicked();
    void exitBnClicked();
    void helpBnClicked();
    void closeEvent(QCloseEvent *e);

private:
    void readSettings();
    void writeSettings();
    static QString targetFileFor(const QString &sourceFile);

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
