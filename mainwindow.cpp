#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

#include "newdatabasedlg.h"
#include "builddatabasedlg.h"
#include "createmosaicdlg.h"
#include "picturedatabase.h"
#include "mosaicdetailsdlg.h"
#include "rendermosaicdlg.h"
#include "editdatabasedlg.h"
#include "aboutdlg.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->createDatabaseBn,
            SIGNAL(pressed()),
            this,
            SLOT(createDatabaseBnClicked()));
    connect(ui->newMosaicButton,
            SIGNAL(pressed()),
            this,
            SLOT(newMosaicBnClicked()));
    connect(ui->editDatabaseButton,
            SIGNAL(pressed()),
            this,
            SLOT(editDatabaseBnClicked()));
    connect(ui->aboutButton,
            SIGNAL(pressed()),
            this,
            SLOT(aboutBnClicked()));
    connect(ui->exitButton,
            SIGNAL(pressed()),
            this,
            SLOT(exitBnClicked()));
    this->readSettings();
}

MainWindow::~MainWindow()
{
    this->writeSettings();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    this->writeSettings();
    e->accept();
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
    NewDatabaseDlg newDBDlg(this);
    newDBDlg.show();
    newDBDlg.exec();
    if (newDBDlg.exitedCorrectly()) {
        BuildDatabaseDlg buildDlg(this,
                                  newDBDlg.name(),
                                  newDBDlg.directory(),
                                  newDBDlg.includeSubdirectories()
                                  );
        buildDlg.show();
        buildDlg.exec();
    }
}

void MainWindow::newMosaicBnClicked()
{
    CreateMosaicDlg createDlg(this);
    createDlg.show();
    createDlg.exec();
    if (createDlg.exitedCorrectly()) {
        PictureDatabase database;
        if (!database.fromFile(createDlg.database())) {
            QMessageBox::warning(this,
                                 tr("Error"),
                                 tr("Could not load the selected database!"),
                                 QMessageBox::Ok);
            return;
        }
        MosaicDetailsDlg detailsDlg(this, createDlg.image());
        detailsDlg.show();
        detailsDlg.exec();
        if (detailsDlg.exitedCorrectly()) {
            RenderMosaicDlg renderDlg(this,
                                      &database,
                                      createDlg.image(),
                                      detailsDlg.tileWidth(),
                                      detailsDlg.tileHeight(),
                                      detailsDlg.tileCount(),
                                      detailsDlg.cutEdges(),
                                      detailsDlg.alphaChannel(),
                                      createDlg.outputImage());
            renderDlg.show();
            renderDlg.exec();
        }
    }
}

void MainWindow::editDatabaseBnClicked()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("InputMainWindow");
    QString databaseFile = settings->value("databasefiletoedit",
                                           QDir::toNativeSeparators(
                                                   QDir::cleanPath(QDir::homePath()))).toString();
    settings->endGroup();
    databaseFile = QFileDialog::getOpenFileName(this,
                                                tr("Select database file"),
                                                QDir::toNativeSeparators(QDir::cleanPath(
                                                        databaseFile)),
                                                tr("Database files (*.mib);;All files (*.*)"));
    if (databaseFile.isNull() || (databaseFile.length()==0)) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("You have to select a valid image database to edit!"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    settings->beginGroup("InputMainWindow");
    settings->setValue("databasefiletoedit", databaseFile);
    settings->endGroup();

    EditDatabaseDlg editDBDlg(this, databaseFile);
    editDBDlg.show();
    editDBDlg.exec();
}

void MainWindow::aboutBnClicked()
{
    AboutDlg aboutDlg;
    aboutDlg.show();
    aboutDlg.exec();
}

void MainWindow::exitBnClicked()
{
    this->writeSettings();
    this->close();
}

void MainWindow::writeSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateMainWindow");
    settings->setValue("size", this->size());
    settings->setValue("pos", this->pos());
    settings->endGroup();
}

void MainWindow::readSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateMainWindow");
    this->resize(settings->value("size", QSize(600, 400)).toSize());
    this->move(settings->value("pos", QPoint(90, 90)).toPoint());
    settings->endGroup();
}
