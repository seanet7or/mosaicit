/***************************************************************************************************
*
* FILE:     indexdirdlg.cpp
*
* CREATED:  07-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  this dialog shows the progress when indexing the files in a directory
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#include "indexdirdlg.h"
#include "ui_indexdirdlg.h"

#include <QMessageBox>
#include <QCloseEvent>

#include "appsettings.h"

IndexDirDlg::IndexDirDlg(QWidget *parent,
                         PictureDatabase *database,
                         const QString &newDir) :
    QDialog(parent),
    ui(new Ui::IndexDirDlg)
{
    ui->setupUi(this);
    this->m_database = database;
    bool subDirs = false;
    if (QMessageBox::question(this,
                              tr("Include subdirectories?"),
                              tr("Do you also want to add the image files in the subdirectories?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        subDirs = true;
    } else {
        subDirs = false;
    }
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(onCancelButtonPressed()));
    connect(database,
            SIGNAL(indexFinished()),
            this,
            SLOT(onIndexingFinished()));
    database->indexFiles(newDir, subDirs);
    this->readSettings();
}

IndexDirDlg::~IndexDirDlg()
{
    this->writeSettings();
    delete ui;
}

void IndexDirDlg::onCancelButtonPressed()
{
    if (this->m_database->isIndexingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel indexing?"),
                                  tr("Do you want to cancel? Not all files in the selected directory will be added!"),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            this->m_database->cancelIndexing();
        }
    }
}

void IndexDirDlg::reject()
{
    this->onCancelButtonPressed();
}

void IndexDirDlg::closeEvent(QCloseEvent *e)
{
    if (this->m_database->isIndexingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel indexing?"),
                                  tr("Do you want to cancel? Not all files in the selected directory will be added!"),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            this->m_database->cancelIndexing();
            this->writeSettings();
            e->accept();
        } else {
            e->ignore();
        }
    } else {
        this->writeSettings();
        e->accept();
    }
}

void IndexDirDlg::onIndexingFinished()
{
    this->writeSettings();
    done(0);
}

void IndexDirDlg::changeEvent(QEvent *e)
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

void IndexDirDlg::writeSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateIndexDirDlg");
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.endGroup();
}

void IndexDirDlg::readSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateIndexDirDlg");
    this->resize(settings.value("size", QSize(400, 91)).toSize());
    this->move(settings.value("pos", QPoint(225, 295)).toPoint());
    settings.endGroup();
}
