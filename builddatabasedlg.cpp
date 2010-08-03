#include "builddatabasedlg.h"
#include "ui_builddatabasedlg.h"

#include <QMessageBox>
#include <QFileDialog>

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
    ui->textLog->appendPlainText(tr("Indexing files in directory ")
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
}

BuildDatabaseDlg::~BuildDatabaseDlg()
{
    delete ui;
}

void BuildDatabaseDlg::closeEvent(QCloseEvent *e)
{
    if (this->m_newDatabase->isIndexingRunning() ||
        this->m_newDatabase->isProcessingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel?"),
                                  tr("Do you really want to cancel? The database will be %1").arg(
                                          "incomplete!"),
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
            e->accept();
        } else {
            e->ignore();
        }
    } else {
        e->accept();
    }

}

void BuildDatabaseDlg::closeButtonPressed()
{
    if (this->m_newDatabase->isIndexingRunning() ||
        this->m_newDatabase->isProcessingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel?"),
                                  tr("Do you really want to cancel? The database will be %1").arg(
                                          "incomplete!"),
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
        done(0);
    }
}

void BuildDatabaseDlg::reject()
{
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
    done(0);
}

void BuildDatabaseDlg::indexingFinished()
{
    if (this->m_canceled) return;
    ui->textLog->appendPlainText(tr("Indexing files finished."));
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
                tr("Saving database file %1").arg(this->m_name));
        if ((saved = this->m_newDatabase->toFile(this->m_name)) == false) {
            if (QMessageBox::question(this,
                                      tr("Error"),
                                      tr("Could not write database file! Do you want to %1").arg(
                                              "specify another one?"),
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
    ui->textLog->appendPlainText(tr("Saved database."));
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
