#include "createmosaicdlg.h"
#include "ui_createmosaicdlg.h"

#include <QFileDialog>

CreateMosaicDlg::CreateMosaicDlg(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CreateMosaicDlg)
{
    ui->setupUi(this);
    connect(ui->selectDatabase,
            SIGNAL(pressed()),
            this,
            SLOT(selectDBBnPressed()));
    connect(ui->selectImage,
            SIGNAL(pressed()),
            this,
            SLOT(selectImageBnPressed()));
    connect(ui->imageEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(imageChanged()));
}

void CreateMosaicDlg::imageChanged()
{
    QImage image(ui->imageEdit->text());
    if (image.isNull()) {
        ui->previewLabel->clear();
        return;
    }
    int lw = ui->previewLabel->width();
    int lh = ui->previewLabel->height();
    ui->previewLabel->setPixmap(
            QPixmap::fromImage(image.scaled(QSize(lw, lh),
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation)));
}

CreateMosaicDlg::~CreateMosaicDlg()
{
    delete ui;
}

void CreateMosaicDlg::changeEvent(QEvent *e)
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

void CreateMosaicDlg::selectDBBnPressed()
{
    ui->databaseEdit->setText(
            QFileDialog::getOpenFileName(this,
                                         tr("Select database file"),
                                         ui->databaseEdit->text(),
                                         tr("Database files (*.mib);;All files (*.*)")));
}

void CreateMosaicDlg::selectImageBnPressed()
{
    ui->imageEdit->setText(
            QFileDialog::getOpenFileName(this,
                                         tr("Select image file"),
                                         ui->imageEdit->text(),
                                         tr("Images (*.png *.bmp *.xpm *.jpg);;All files (*.*")));
}
