/***************************************************************************************************
*
* FILE:     builddatabasedlg.cpp
*
* CREATED:  01-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  Dialog that shows the progress when building a databas
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#include "builddatabasedlg.h"
#include "ui_builddatabasedlg.h"

#include <QMessageBox>
#include <QFileDialog>

#include "appsettings.h"

BuildDatabaseDlg::BuildDatabaseDlg(QWidget *parent,
                                   const QString &name,
                                   const QString &directory,
                                   bool includeSubdirectories) :
QDialog(parent),
ui(new Ui::BuildDatabaseDlg)
{
    ui->setupUi(this);
    this->m_name = name;
    this->m_directory = directory;
    this->m_includeSubdirectories = includeSubdirectories;
    this->m_newDatabase = new PictureDatabase;
    this->m_newDatabase->setName(name);
    this->m_canceled = false;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    ui->textLog->appendPlainText(tr("Indexing the files in the directory ")
                                 + this->m_directory);
    connect(this->m_newDatabase,
            SIGNAL(indexFinished()),
            this,
            SLOT(indexingFinished()));
    this->m_newDatabase->indexFiles(this->m_directory,
                                    this->m_includeSubdirectories);
    connect(ui->closeButton,
            SIGNAL(pressed()),
            this,
            SLOT(closeButtonPressed()));
    readSettings();
}

BuildDatabaseDlg::~BuildDatabaseDlg()
{
    this->writeSettings();
    delete ui;
}

void BuildDatabaseDlg::closeEvent(QCloseEvent *e)
{
    if (this->m_newDatabase->isIndexingRunning() ||
        this->m_newDatabase->isProcessingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel?"),
                                  tr("Do you really want to cancel? The database will be incomplete!"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No) == QMessageBox::Yes) {
            this->m_canceled = true;
            if (this->m_newDatabase->isIndexingRunning()) {
                this->m_newDatabase->cancelIndexing();
            }
            if (this->m_newDatabase->isProcessingRunning()) {
                this->m_newDatabase->cancelProcessing();
            }
            ui->label->setText(tr("Building the database was canceled!"));
            if (ui->progressBar->minimum() == ui->progressBar->maximum()) {
                ui->progressBar->setMaximum(100);
                ui->progressBar->setMinimum(0);
                ui->progressBar->setValue(0);
            }
            ui->progressBar->setEnabled(false);
            ui->closeButton->setText(tr("Close"));
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

void BuildDatabaseDlg::closeButtonPressed()
{
    if (this->m_newDatabase->isIndexingRunning() ||
        this->m_newDatabase->isProcessingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel?"),
                                  tr("Do you really want to cancel? The database will be incomplete!"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No) == QMessageBox::Yes) {
            this->m_canceled = true;
            if (this->m_newDatabase->isIndexingRunning()) {
                this->m_newDatabase->cancelIndexing();
            }
            if (this->m_newDatabase->isProcessingRunning()) {
                this->m_newDatabase->cancelProcessing();
            }
            ui->label->setText(tr("The build process was canceled!"));
            if (ui->progressBar->minimum() == ui->progressBar->maximum()) {
                ui->progressBar->setMaximum(100);
                ui->progressBar->setMinimum(0);
                ui->progressBar->setValue(0);
            }
            ui->progressBar->setEnabled(false);
            ui->closeButton->setText(tr("Close"));
        }
    } else {
        this->writeSettings();
        done(0);
    }
}

void BuildDatabaseDlg::reject()
{
    if (QMessageBox::question(this,
                              tr("Do you want to cancel?"),
                              tr("Do you really want to cancel? The database will not be saved!"),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }
    disconnect(this->m_newDatabase,
               SIGNAL(indexFinished()),
               this,
               SLOT(indexingFinished()));
    disconnect(this->m_newDatabase,
               SIGNAL(processFinished(bool)),
               this,
               SLOT(processingFinished(bool)));
    disconnect(this->m_newDatabase,
               SIGNAL(processProgress(float)),
               this,
               SLOT(processProgress(float)));
    if (this->m_newDatabase->isIndexingRunning()) {
        this->m_newDatabase->cancelIndexing();
    }
    if (this->m_newDatabase->isProcessingRunning()) {
        this->m_newDatabase->cancelProcessing();
    }
    this->writeSettings();
    done(0);
}

void BuildDatabaseDlg::indexingFinished()
{
    if (this->m_canceled) return;
    ui->textLog->appendPlainText(tr("Indexing of the files is finished."));
    connect(this->m_newDatabase,
            SIGNAL(processFinished(bool)),
            this,
            SLOT(processingFinished(bool)));
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    connect(this->m_newDatabase,
            SIGNAL(processProgress(float)),
            this,
            SLOT(processProgress(float)));
    ui->textLog->appendPlainText(tr("Analyzing all found image files..."));
    this->m_newDatabase->processFiles();
}

void BuildDatabaseDlg::processingFinished(bool wasCanceled)
{
    if (!wasCanceled) {
        ui->textLog->appendPlainText(tr("Analyzing found files finished."));
    } else {
        ui->textLog->appendPlainText(tr("Analyzing was canceled!"));
        ui->textLog->appendPlainText(tr("The database will be incomplete."));
        ui->textLog->appendPlainText(tr("Use \"Edit Database\" to analyse the missing files."));
    }
    bool saved = false;
    while (!saved) {
        ui->textLog->appendPlainText(
                tr("Saving the database as %1").arg(this->m_name));
        if ((saved = this->m_newDatabase->toFile(this->m_name)) == false) {
            if (QMessageBox::question(this,
                                      tr("Error"),
                                      tr("Could not write the database file! Do you want to specify another one?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes) == QMessageBox::Yes) {
                this->m_name = QFileDialog::getSaveFileName(this,
                                                            tr("Database file to write"),
                                                            QDir::toNativeSeparators(
                                                                    QDir::cleanPath(
                                                                            this->m_name)),
                                                            tr("Database file (*.mib)"));
            } else {
                ui->textLog->appendPlainText(
                        tr("Error saving the database file."));
                ui->label->setText(tr("The database could not be written."));
                ui->closeButton->setText(tr("Close"));
                return;
            }
        }
    }
    ui->textLog->appendPlainText(tr("The database was saved."));
    if (!wasCanceled) {
        ui->label->setText(tr("The database was built successfully."));
    } else {
        ui->label->setText(tr("The database will be incomplete!"));
    }
    ui->closeButton->setText(tr("Close"));
}

void BuildDatabaseDlg::changeEvent(QEvent *e)
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

void BuildDatabaseDlg::processProgress(float percent)
{
    ui->progressBar->setValue((int)percent);
}

void BuildDatabaseDlg::writeSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateBuildDatabaseDlg");
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.endGroup();
}

void BuildDatabaseDlg::readSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateBuildDatabaseDlg");
    this->resize(settings.value("size", QSize(370, 234)).toSize());
    this->move(settings.value("pos", QPoint(310, 270)).toPoint());
    settings.endGroup();
}
