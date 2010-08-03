#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

private slots:
    void createDatabaseBnClicked();
    void newMosaicBnClicked();
    void editDatabaseBnClicked();
    void aboutBnClicked();
    void exitBnClicked();
};

#endif // MAINWINDOW_H
