#include "editdatabasedlg.h"
#include "ui_editdatabasedlg.h"

EditDatabaseDlg::EditDatabaseDlg(QWidget *parent, const QString &databaseFile) :
    QDialog(parent),
    ui(new Ui::EditDatabaseDlg)
{
    ui->setupUi(this);
    this->m_database = new PictureDatabase;
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
