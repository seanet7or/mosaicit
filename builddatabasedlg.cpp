#include "builddatabasedlg.h"
#include "ui_builddatabasedlg.h"

BuildDatabaseDlg::BuildDatabaseDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuildDatabaseDlg)
{
    ui->setupUi(this);
}

BuildDatabaseDlg::~BuildDatabaseDlg()
{
    delete ui;
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
