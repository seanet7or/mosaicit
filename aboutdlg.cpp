#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    connect(ui->pushButton,
            SIGNAL(pressed()),
            this,
            SLOT(onOkButtonPressed()));
}

AboutDlg::~AboutDlg()
{
    delete ui;
}

void AboutDlg::changeEvent(QEvent *e)
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

void AboutDlg::onOkButtonPressed()
{
    done(0);
}
