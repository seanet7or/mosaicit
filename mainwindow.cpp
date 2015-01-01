/***************************************************************************************************
*
* FILE:     mainwindow.cpp
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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QDebug>
#include <QStandardPaths>
#include "newdatabasedlg.h"
#include "database/picturedatabase.h"
#include "mosaicdetailsdlg.h"
#include "rendermosaicdlg.h"
#include "aboutdlg.h"
#include "picturedatabasedlg.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
	qDebug() << "Creating main window";
    ui->setupUi(this);

    ui->newMosaicBn->setText(tr("Create New Mosaic"));
    ui->pictureLibraryBn->setText(tr("Edit Picture Library"));
    ui->aboutBn->setText(tr("About MosaicIt!"));

    QPalette* palette = new QPalette();
    palette->setBrush(QPalette::Window,
                      QBrush(QImage(":/app_logo")));
    setPalette(*palette);

    connect(ui->newMosaicBn,
            &Button::pressed,
            this,
            &MainWindow::newMosaicBnClicked);
    connect(ui->aboutBn,
            &Button::pressed,
            this,
            &MainWindow::aboutBnClicked);
    connect(ui->pictureLibraryBn,
            &Button::pressed,
            this,
            &MainWindow::pictureLibraryBnClicked);
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

void MainWindow::showEvent(QShowEvent *)
{

}

void MainWindow::pictureLibraryBnClicked()
{
    PictureDatabaseDlg dlg;
    dlg.show();
    dlg.exec();
}

void MainWindow::helpBnClicked()
{
    QString command;
#ifdef Q_OS_WIN32
    command = QString("\"") + qApp->applicationDirPath() + "/starthelp.exe" + QString("\"");
#endif
    QProcess::startDetached(command);
}

QString MainWindow::targetFileFor(const QString &sourceFile)
{
    QString extension = sourceFile.split('.').last();
    QString targetFile = sourceFile;
    int pointPos = targetFile.lastIndexOf('.');
    if (pointPos != -1) {
        targetFile = targetFile.left(pointPos);
    }
    targetFile.append("Mosaic");

    QFileInfo targetFileInfo(targetFile + "." + extension);
    if (targetFileInfo.exists())
    {
        int index = 1;
        bool fileExists = false;
        do
        {
            QString strIndex = QString::number(index);
            if (strIndex.length() < 2)
            {
                strIndex = "0" + strIndex;
            }
            QFileInfo targetFileWithIndexInfo(targetFile + "-" + strIndex + "." + extension);
            fileExists = targetFileWithIndexInfo.exists();
            if (!fileExists) {
                targetFile += "-" + strIndex;
            } else {
                index++;
            }
        }
        while (fileExists);
    }

    targetFile.append("." + extension);
    return targetFile;
}

void MainWindow::newMosaicBnClicked()
{
    QString startDir;
    QStringList pictureLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    if (pictureLocations.length() > 0)
    {
        startDir = pictureLocations.first();
    }
    else
    {
        QStringList documentLocations =
                QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        if (documentLocations.length() > 0)
        {
            startDir = documentLocations.first();
        }
        else
        {
            startDir = QString();
        }
    }

    QString imageFile = QFileDialog::getOpenFileName(this,
                                                     tr("Select original image"),
                                                     startDir,
                                                     tr("Images (*.png *.bmp *.xpm *.jpg *.jpeg);;"\
                                                        "All files (*.*)")
                                                     );
    if ((imageFile != NULL) && (imageFile.length() > 0))
    {
        imageFile = QDir::toNativeSeparators(QDir::cleanPath(imageFile));
        MosaicDetailsDlg detailsDlg(this, imageFile);
        detailsDlg.show();
        detailsDlg.exec();
        if (detailsDlg.exitedCorrectly()) {
            QString targetFile = targetFileFor(imageFile);

            RenderMosaicDlg renderDlg(this,
                                      imageFile,
                                      detailsDlg.tileWidth(),
                                      detailsDlg.tileHeight(),
                                      detailsDlg.tileCount(),
                                      detailsDlg.cutEdges(),
                                      detailsDlg.alphaChannel(),
                                      targetFile,
                                      detailsDlg.minDistanceChecker(),
                                      detailsDlg.minDistance(),
                                      detailsDlg.repeatTilesMaxChecker(),
                                      detailsDlg.repeatTilesMaxCount());
            renderDlg.show();
            renderDlg.exec();
        }
    }
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
    QSettings settings;
    settings.beginGroup("GUIStateMainWindow");
    //settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateMainWindow");
    //this->resize(settings.value("size", QSize(670, 545)).toSize());
    this->move(settings.value("pos", QPoint(90, 90)).toPoint());
    settings.endGroup();
}
