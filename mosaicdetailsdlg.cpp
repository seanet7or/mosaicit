#include "mosaicdetailsdlg.h"
#include "ui_mosaicdetailsdlg.h"

MosaicDetailsDlg::MosaicDetailsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MosaicDetailsDlg)
{
    ui->setupUi(this);
}

MosaicDetailsDlg::~MosaicDetailsDlg()
{
    delete ui;
}

void MosaicDetailsDlg::changeEvent(QEvent *e)
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
