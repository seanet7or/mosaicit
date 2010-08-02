#include "editdatabasedlg.h"
#include "ui_editdatabasedlg.h"

#include <QMessageBox>
#include <QPainter>

#include "updatedatabasedlg.h"

EditDatabaseDlg::EditDatabaseDlg(QWidget *parent, const QString &databaseFile) :
        QDialog(parent),
        ui(new Ui::EditDatabaseDlg)
{
    ui->setupUi(this);

    //load database and prepare if necessary
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

    connect(ui->fileList,
            SIGNAL(currentRowChanged(int)),
            this,
            SLOT(onFileSelected(int)));

    //update ui elements
    ui->databaseInfoLabel->setText(
            tr("%1 files in database \"%2\"; %3 do not exist or are untracked yet").arg(
                    QString::number(this->m_database->size()),
                    databaseFile,
                    QString::number(this->m_database->filesNotUpToDate())));
    for (int i = 0; i < this->m_database->size(); i++) {
        ui->fileList->addItem(this->m_database->pictureAt(i)->getFile());
    }
}

void EditDatabaseDlg::onFileSelected(int row)
{
    if ((row >= 0) && (row < this->m_database->size())) {
        ui->imagePathLabel->setText(this->m_database->pictureAt(row)->getFile());;
        if (this->m_database->pictureAt(row)->validFile()) {
            int lw = ui->imagePreview->width();
            int lh = ui->imagePreview->height();
            QImage image(this->m_database->pictureAt(row)->getFile());
            if (!image.isNull()) {
                ui->imagePreview->setPixmap(
                        QPixmap::fromImage(image.scaled(QSize(lw, lh),
                                                        Qt::KeepAspectRatio,
                                                        Qt::SmoothTransformation)));
            } else {
                ui->imagePreview->clear();
            }
            if (this->m_database->pictureAt(row)->processed()) {
                //file was already processed
                ui->processedLabel->setText(tr("The average color is (%1, %2, %3)").arg(
                        QString::number(this->m_database->pictureAt(row)->getRed()),
                        QString::number(this->m_database->pictureAt(row)->getGreen()),
                        QString::number(this->m_database->pictureAt(row)->getBlue())));
            } else {
                //file not processed
                ui->processedLabel->setText(tr("This file was not analysed yet."));
            }
        } else {
            //no valid file
            ui->imagePreview->clear();
            ui->processedLabel->setText(tr("This seems not to be a valid image file."));
        }
    } else {
        //no valid row
        ui->imagePreview->clear();
        ui->imagePathLabel->clear();
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
