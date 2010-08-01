#include "createmosaicdlg.h"
#include "ui_createmosaicdlg.h"

#include <QFileDialog>
#include <QImage>
#include <QFile>
#include <QMessageBox>

CreateMosaicDlg::CreateMosaicDlg(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CreateMosaicDlg)
{
    ui->setupUi(this);
    this->m_canceled = true;
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
    connect(ui->nextButton,
            SIGNAL(pressed()),
            this,
            SLOT(nextBnPressed()));
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(cancelBnPressed()));
}

bool CreateMosaicDlg::exitedCorrectly()
{
    return !this->m_canceled;
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

void CreateMosaicDlg::cancelBnPressed()
{
    done(0);
}

void CreateMosaicDlg::nextBnPressed()
{
    QFile dbFile(ui->databaseEdit->text());
    if (!dbFile.exists()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Select a database file!"),
                             QMessageBox::Ok);
        return;
    }
    QImage image(ui->imageEdit->text());
    if (image.isNull()) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Select a valid image file!"),
                             QMessageBox::Ok);
        return;
    }
    this->m_canceled = false;
    this->m_image = ui->imageEdit->text();
    this->m_database = ui->databaseEdit->text();
    done(0);
}

QString CreateMosaicDlg::image()
{
    return this->m_image;
}

QString CreateMosaicDlg::database()
{
    return this->m_database;
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
