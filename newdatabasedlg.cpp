#include "newdatabasedlg.h"
#include "ui_newdatabasedlg.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFile>

#include "appsettings.h"

NewDatabaseDlg::NewDatabaseDlg(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::NewDatabaseDlg)
{
    ui->setupUi(this);
    m_canceled = true;

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
    this->readSettings();
}

void NewDatabaseDlg::selectFileButtonPressed()
{
    ui->nameEdit->setText(
            QDir::toNativeSeparators(
                    QDir::cleanPath(
                            QFileDialog::getSaveFileName(this,
                                                         tr("Database file to write"),
                                                         QDir::toNativeSeparators(
                                                                 QDir::cleanPath(
                                                                         ui->nameEdit->text())),
                                                         tr("Database file (*.mib)")
                                                         ))));
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
    QFileInfo outFileInfo(ui->nameEdit->text());
    if (outFileInfo.exists()) {
        if (QMessageBox::question(this,
                                  tr("Database file exists"),
                                  tr("The chosen database file already exists. Do you want%1").arg(
                                          tr(" to override it?")),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
            return;
        }
    }
    QFile outFile(ui->nameEdit->text());
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("The database file you specified can not be written. %1").arg(
                                     "Specify another one."),
                             QMessageBox::Ok);
        return;
    }
    outFile.close();
    outFile.remove();
    m_canceled = false;
    this->m_directory = ui->directoryEdit->text();
    this->m_name = ui->nameEdit->text();
    this->m_includeSubdirs = ui->includeSubdirectories->isChecked();
    this->writeSettings();
    close();
}

void NewDatabaseDlg::cancelButtonPressed()
{
    this->writeSettings();
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
            this->writeSettings();
            e->accept();
        }
    } else {
        this->writeSettings();
        e->accept();
    }
}

void NewDatabaseDlg::selectDirButtonPressed()
{
    ui->directoryEdit->setText(QDir::toNativeSeparators(QDir::cleanPath(
            QFileDialog::getExistingDirectory(
                    this,
                    tr("Select picture folder"),
                    QDir::toNativeSeparators(QDir::cleanPath(ui->directoryEdit->text())),
                    QFileDialog::ShowDirsOnly))));
}

NewDatabaseDlg::~NewDatabaseDlg()
{
    this->writeSettings();
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

void NewDatabaseDlg::writeSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateNewDatabaseDlg");
    settings->setValue("size", this->size());
    settings->setValue("pos", this->pos());
    settings->endGroup();
    settings->beginGroup("InputNewDatabaseDlg");
    settings->setValue("newdatabasefile", ui->nameEdit->text());
    settings->setValue("picturesdirectory", ui->directoryEdit->text());
    settings->setValue("includesubdirectories", ui->includeSubdirectories->isChecked());
    settings->endGroup();
}

void NewDatabaseDlg::readSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateNewDatabaseDlg");
    this->resize(settings->value("size", QSize(596, 140)).toSize());
    this->move(settings->value("pos", QPoint(180, 280)).toPoint());
    settings->endGroup();
    settings->beginGroup("InputNewDatabaseDlg");
    ui->nameEdit->setText(settings->value("newdatabasefile",
                                          QDir::toNativeSeparators(
                                                  QDir::cleanPath(
                                                          QDir::homePath()
                                                          + tr("/newdatabase.mib")))).toString());
    ui->directoryEdit->setText(settings->value("picturesdirectory",
                                               QDir::toNativeSeparators(
                                                       QDir::cleanPath(
                                                               QDir::homePath()))).toString());
    ui->includeSubdirectories->setChecked(settings->value("includesubdirectories", true).toBool());
    settings->endGroup();
}
