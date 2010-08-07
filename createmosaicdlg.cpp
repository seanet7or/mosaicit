/***************************************************************************************************
*
* FILE:     createmosaicdlg.cpp
*
* CREATED:  01-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  Dialog that lets the user create a new mosaic
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#include "createmosaicdlg.h"
#include "ui_createmosaicdlg.h"

#include <QFileDialog>
#include <QImage>
#include <QFile>
#include <QMessageBox>

#include "appsettings.h"

CreateMosaicDlg::CreateMosaicDlg(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CreateMosaicDlg)
{
    ui->setupUi(this);
    this->m_canceled = true;
    setTabOrder(ui->databaseEdit, ui->selectDatabase);
    setTabOrder(ui->selectDatabase, ui->imageEdit);
    setTabOrder(ui->imageEdit, ui->selectImage);
    setTabOrder(ui->selectImage, ui->outputEdit);
    setTabOrder(ui->outputEdit, ui->selectOutput);
    setTabOrder(ui->selectOutput, ui->nextButton);
    setTabOrder(ui->nextButton, ui->cancelButton);
    connect(ui->selectDatabase,
            SIGNAL(pressed()),
            this,
            SLOT(selectDBBnPressed()));
    connect(ui->selectImage,
            SIGNAL(pressed()),
            this,
            SLOT(selectImageBnPressed()));
    connect(ui->imageEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(imageChanged()));
    connect(ui->nextButton,
            SIGNAL(pressed()),
            this,
            SLOT(nextBnPressed()));
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(cancelBnPressed()));
    connect(ui->selectOutput,
            SIGNAL(pressed()),
            this,
            SLOT(selectOutputBnPressed()));
    this->readSettings();
}

bool CreateMosaicDlg::exitedCorrectly()
{
    return !this->m_canceled;
}

void CreateMosaicDlg::imageChanged()
{
    QImage image(ui->imageEdit->text());
    if (image.isNull()) {
        ui->previewLabel->clear();
        return;
    }
    int lw = ui->previewLabel->width();
    int lh = ui->previewLabel->height();
    ui->previewLabel->setPixmap(
            QPixmap::fromImage(image.scaled(QSize(lw, lh),
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation)));
}

CreateMosaicDlg::~CreateMosaicDlg()
{
    this->writeSettings();
    delete ui;
}

void CreateMosaicDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CreateMosaicDlg::cancelBnPressed()
{
    this->writeSettings();
    done(0);
}

void CreateMosaicDlg::nextBnPressed()
{
    QFile dbFile(ui->databaseEdit->text());
    if (!dbFile.exists()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Select a database file!"),
                             QMessageBox::Ok);
        return;
    }
    QImage image(ui->imageEdit->text());
    if (image.isNull()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Select a valid image file!"),
                             QMessageBox::Ok);
        return;
    }
    QFileInfo outInfo(ui->outputEdit->text());
    if ((QString(ui->outputEdit->text()).length() == 0) || (outInfo.isDir())) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Select a mosaic output image file!"),
                             QMessageBox::Ok);
        return;
    }
    this->m_canceled = false;
    this->m_image = ui->imageEdit->text();
    this->m_database = ui->databaseEdit->text();
    this->m_outputImage = ui->outputEdit->text();
    this->writeSettings();
    done(0);
}

QString CreateMosaicDlg::image()
{
    return this->m_image;
}

QString CreateMosaicDlg::database()
{
    return this->m_database;
}

QString CreateMosaicDlg::outputImage()
{
    return this->m_outputImage;
}

void CreateMosaicDlg::selectDBBnPressed()
{
    ui->databaseEdit->setText(
            QDir::toNativeSeparators(
                    QDir::cleanPath(
                            QFileDialog::getOpenFileName(this,
                                                         tr("Select database file"),
                                                         QDir::toNativeSeparators(
                                                                 QDir::cleanPath(
                                                                         ui->databaseEdit->text())),
                                                         tr("Database files (*.mib);;All files (*.*)")
                                                         ))));
}

void CreateMosaicDlg::selectOutputBnPressed()
{
    ui->outputEdit->setText(
            QDir::toNativeSeparators(
                    QDir::cleanPath(
                            QFileDialog::getSaveFileName(this,
                                                         tr("Select mosaic output file"),
                                                         QDir::toNativeSeparators(
                                                                 QDir::cleanPath(
                                                                         ui->outputEdit->text())),
                                                         tr("Images (*.jpg)")))));
}

void CreateMosaicDlg::selectImageBnPressed()
{
    ui->imageEdit->setText(
            QDir::toNativeSeparators(
                    QDir::cleanPath(
                            QFileDialog::getOpenFileName(this,
                                                         tr("Select original image"),
                                                         QDir::toNativeSeparators(
                                                                 QDir::cleanPath(
                                                                         ui->imageEdit->text())),
                                                         tr("Images (*.png *.bmp *.xpm *.jpg);;All files (*.*)")))));
}

void CreateMosaicDlg::writeSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateCreateMosaicDlg");
    settings->setValue("size", this->size());
    settings->setValue("pos", this->pos());
    settings->endGroup();
    settings->beginGroup("InputCreateMosaicDlg");
    settings->setValue("databasefile", ui->databaseEdit->text());
    settings->setValue("imageinputfile", ui->imageEdit->text());
    settings->setValue("outputfile", ui->outputEdit->text());
    settings->endGroup();
}

void CreateMosaicDlg::readSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateCreateMosaicDlg");
    this->resize(settings->value("size", QSize(644, 222)).toSize());
    this->move(settings->value("pos", QPoint(25, 135)).toPoint());
    settings->endGroup();
    settings->beginGroup("InputCreateMosaicDlg");
    ui->databaseEdit->setText(
            settings->value("databasefile",
                            QDir::toNativeSeparators(QDir::cleanPath(
                                    QDir::homePath()))).toString());
    ui->imageEdit->setText(
            settings->value("imageinputfile",
                            QDir::toNativeSeparators(QDir::cleanPath(
                                    QDir::homePath()))).toString());;
    ui->outputEdit->setText(
            settings->value("outputfile",
                            QDir::toNativeSeparators(QDir::cleanPath(
                                    QDir::homePath() + "/mosaic.jpg"))).toString());
    settings->endGroup();
}
