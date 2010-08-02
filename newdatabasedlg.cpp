#include "newdatabasedlg.h"
#include "ui_newdatabasedlg.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFile>

NewDatabaseDlg::NewDatabaseDlg(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::NewDatabaseDlg)
{
    ui->setupUi(this);
    m_canceled = true;
    ui->directoryEdit->setText(QDir::homePath());
    ui->nameEdit->setText(QDir::homePath() + tr("/newdatabase.mib"));
    ui->includeSubdirectories->setChecked(true);

    connect(ui->buildButton,
            SIGNAL(pressed()),
            this,
            SLOT(buildButtonPressed()));
    connect(ui->selectDirButton,
            SIGNAL(pressed()),
            this,
            SLOT(selectDirButtonPressed()));
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(cancelButtonPressed()));
    connect(ui->selectFileButton,
            SIGNAL(pressed()),
            this,
            SLOT(selectFileButtonPressed()));
}

void NewDatabaseDlg::selectFileButtonPressed()
{
    ui->nameEdit->setText(
            QFileDialog::getSaveFileName(this,
                                         tr("Database file to write"),
                                         ui->nameEdit->text(),
                                         tr("Database file (*.mib)")
                                         ));
}

void NewDatabaseDlg::buildButtonPressed()
{
    QDir rootDir(ui->directoryEdit->text());
    if ("" == ui->nameEdit->text()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("You have to specify a file for the new database."),
                             QMessageBox::Ok);
        return;
    } else if (!rootDir.exists()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("You have to specify a valid directory."),
                             QMessageBox::Ok);
        return;
    }
    QFile outFile(ui->nameEdit->text());
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("The database file you specified can not be written. Specify another one."),
                             QMessageBox::Ok);
        return;
    }
    outFile.close();
    outFile.remove();
    m_canceled = false;
    this->m_directory = ui->directoryEdit->text();
    this->m_name = ui->nameEdit->text();
    this->m_includeSubdirs = ui->includeSubdirectories->isChecked();
    close();
}

void NewDatabaseDlg::cancelButtonPressed()
{
    close();
}

void NewDatabaseDlg::closeEvent(QCloseEvent *e)
{
    if (m_canceled) {
        if (QMessageBox::question(this,
                                  tr("Cancel?"),
                                  tr("Do you really wont to cancel?"),
                                  QMessageBox::No | QMessageBox::Yes,
                                  QMessageBox::No) != QMessageBox::Yes) {
            e->ignore();
        } else {
            e->accept();
        }
    } else {
        e->accept();
    }
}

void NewDatabaseDlg::selectDirButtonPressed()
{
    ui->directoryEdit->setText(QFileDialog::getExistingDirectory(
            this,
            tr("Select picture folder"),
            ui->directoryEdit->text(),
            QFileDialog::ShowDirsOnly));
}

NewDatabaseDlg::~NewDatabaseDlg()
{
    delete ui;
}

void NewDatabaseDlg::changeEvent(QEvent *e)
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

bool NewDatabaseDlg::exitedCorrectly()
{
    return !this->m_canceled;
}

QString NewDatabaseDlg::name()
{
    return this->m_name;
}

QString NewDatabaseDlg::directory()
{
    return this->m_directory;
}

bool NewDatabaseDlg::includeSubdirectories()
{
    return this->m_includeSubdirs;
}
