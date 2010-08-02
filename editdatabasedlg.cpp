#include "editdatabasedlg.h"
#include "ui_editdatabasedlg.h"

#include <QMessageBox>

#include "updatedatabasedlg.h"

EditDatabaseDlg::EditDatabaseDlg(QWidget *parent, const QString &databaseFile) :
        QDialog(parent),
        ui(new Ui::EditDatabaseDlg)
{
    ui->setupUi(this);
    this->m_database = new PictureDatabase;
    if (!this->m_database->fromFile(databaseFile)) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Could not open the selected database file \"%1\"!").arg(
                                     databaseFile),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
    if (!this->m_database->allUpToDate()) {
        if (QMessageBox::question(this,
                                  tr("Database is incomplete"),
                                  tr("Not all files in the database are alreay scanned %1").arg(
                                          tr("and/or some information is outdated.\n Do %1").arg(
                                                  tr("you want to rebuild the missing parts?"))),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            UpdateDatabaseDlg updateDlg(this, this->m_database);
            updateDlg.show();
            updateDlg.exec();

        }
    }
}

EditDatabaseDlg::~EditDatabaseDlg()
{
    delete ui;
}

void EditDatabaseDlg::changeEvent(QEvent *e)
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
