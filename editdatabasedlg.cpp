#include "editdatabasedlg.h"
#include "ui_editdatabasedlg.h"

EditDatabaseDlg::EditDatabaseDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDatabaseDlg)
{
    ui->setupUi(this);
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
