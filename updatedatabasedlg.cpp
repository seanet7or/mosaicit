#include "updatedatabasedlg.h"
#include "ui_updatedatabasedlg.h"

#include <QMessageBox>
#include <QCloseEvent>

#include "debug.h"

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
    this->m_database->processFiles();
}

UpdateDatabaseDlg::~UpdateDatabaseDlg()
{
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
    log("UpdateDatabaseDlg::processComplete was called!");
    disconnect(this->m_database,
               SIGNAL(processFinished(bool)));
    if (wasCanceled) {
        ui->label->setText(tr("The process was canceled, the database will be incomplete."));
    } else {
        ui->label->setText(tr("Files were analysed!"));
    }
    ui->cancelButton->setText(tr("Close"));
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
                log("\nUpdating the database was canceled!");
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
                log("\nUpdating the database was canceled!");
                e->accept();
            } else {
                e->ignore();
            }
        } else {
            e->accept();
        }
    } else {
        e->accept();
    }
}
