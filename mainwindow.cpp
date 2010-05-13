#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databasewizard/databasewizard.h"
#include "newdatabasedlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->createDatabaseBn,
            SIGNAL(clicked()),
            this,
            SLOT(createDatabaseBnClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::createDatabaseBnClicked()
{
    NewDatabaseDlg newDBDlg;
    newDBDlg.show();
    newDBDlg.exec();
    if (newDBDlg.exitedCorrectly()) {

    }
}
