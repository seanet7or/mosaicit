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
    MainWindow(const QString &appPath, QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void createDatabaseBnClicked();
    void newMosaicBnClicked();
    void editDatabaseBnClicked();
    void aboutBnClicked();
    void exitBnClicked();
    void closeEvent(QCloseEvent *e);

private:
    void readSettings();
    void writeSettings();

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
