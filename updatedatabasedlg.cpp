/***************************************************************************************************
*
* FILE:     updatedatabasedlg.cpp
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  dialog that shows the process when updating a database
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#include "updatedatabasedlg.h"
#include "ui_updatedatabasedlg.h"

#include <QMessageBox>
#include <QCloseEvent>

#include "debug.h"
#include "appsettings.h"

#define log(text) Debug::log(text)

UpdateDatabaseDlg::UpdateDatabaseDlg(QWidget *parent,
                                     PictureDatabase *database) :
        QDialog(parent),
        ui(new Ui::UpdateDatabaseDlg)
{
    ui->setupUi(this);
    this->m_database = database;
    disconnect(this->m_database,
               SIGNAL(processProgress(float)),
               this,
               SLOT(processDone(float)));
    connect(this->m_database,
            SIGNAL(processProgress(float)),
            this,
            SLOT(processDone(float)));
    disconnect(this->m_database,
               SIGNAL(processFinished(bool)),
               this,
               SLOT(processComplete(bool)));
    connect(this->m_database,
            SIGNAL(processFinished(bool)),
            this,
            SLOT(processComplete(bool)));
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(cancelButtonPressed()));
    this->readSettings();
    this->m_database->processFiles();
}

UpdateDatabaseDlg::~UpdateDatabaseDlg()
{
    this->writeSettings();
    delete ui;
}

void UpdateDatabaseDlg::changeEvent(QEvent *e)
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

void UpdateDatabaseDlg::processDone(float percent)
{
    ui->progressBar->setValue((int)percent);
}

void UpdateDatabaseDlg::processComplete(bool wasCanceled)
{
    qDebug() << ("UpdateDatabaseDlg::processComplete was called!");
    disconnect(this->m_database,
               SIGNAL(processFinished(bool)));
    if (wasCanceled) {
        ui->label->setText(tr("Analysing the files was canceled, the database will be incomplete."));
    } else {
        ui->label->setText(tr("All files were analysed!"));
    }
    ui->cancelButton->setText(tr("Close"));
    this->writeSettings();
    done(0);
}

void UpdateDatabaseDlg::cancelButtonPressed()
{
    if (this->m_database) {
        if (this->m_database->isProcessingRunning()) {
            if (QMessageBox::question(this,
                                      tr("Do you want to cancel?"),
                                      tr("Do you want to cancel? The database will be incomplete!"),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                this->m_database->cancelProcessing();
                qDebug() << ("\nUpdating the database was canceled!");
                return;
            }
        } else {
            return;
        }
    }
}

void UpdateDatabaseDlg::reject()
{
    this->cancelButtonPressed();
}

void UpdateDatabaseDlg::closeEvent(QCloseEvent *e)
{
    if (this->m_database) {
        if (this->m_database->isProcessingRunning()) {
            if (QMessageBox::question(this,
                                      tr("Do you want to cancel?"),
                                      tr("Do you want to cancel? The database will be incomplete!"),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                this->m_database->cancelProcessing();
                qDebug() << ("\nUpdating the database was canceled!");
                this->writeSettings();
                e->accept();
            } else {
                e->ignore();
            }
        } else {
            this->writeSettings();
            e->accept();
        }
    } else {
        this->writeSettings();
        e->accept();
    }
}

void UpdateDatabaseDlg::writeSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateUpdateDatabaseDlg");
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.endGroup();
}

void UpdateDatabaseDlg::readSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateUpdateDatabaseDlg");
    this->resize(settings.value("size", QSize(400, 93)).toSize());
    this->move(settings.value("pos", QPoint(180, 330)).toPoint());
    settings.endGroup();
}
