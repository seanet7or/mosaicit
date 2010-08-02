#include "editdatabasedlg.h"
#include "ui_editdatabasedlg.h"

#include <QMessageBox>
#include <QPainter>
#include <QFileDialog>

#include "updatedatabasedlg.h"
#include "indexdirdlg.h"

EditDatabaseDlg::EditDatabaseDlg(QWidget *parent, const QString &databaseFile) :
        QDialog(parent),
        ui(new Ui::EditDatabaseDlg)
{
    ui->setupUi(this);
    this->m_databaseFile = databaseFile;

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

    connect(ui->removeFileButton,
            SIGNAL(pressed()),
            this,
            SLOT(onDelEntryButtonPressed()));
    connect(ui->updateDatabaseButton,
            SIGNAL(pressed()),
            this,
            SLOT(onUpdateDBButtonPressed()));
    connect(ui->fileList,
            SIGNAL(currentRowChanged(int)),
            this,
            SLOT(onFileSelected(int)));
    connect(ui->addDirButton,
            SIGNAL(pressed()),
            this,
            SLOT(onAddDirButtonPressed()));
    connect(ui->adFileButton,
            SIGNAL(pressed()),
            this,
            SLOT(onAddFileButtonPressed()));
    connect(ui->closeButton,
            SIGNAL(pressed()),
            this,
            SLOT(onCloseButtonPressed()));

    this->setWindowTitle(tr("Edit database \"%1\"").arg(this->m_databaseFile));
    //update ui elements
    this->updateUIElements();
}

void EditDatabaseDlg::onUpdateDBButtonPressed()
{
    if (!this->m_database->allUpToDate()) {
        UpdateDatabaseDlg updateDlg(this, this->m_database);
        updateDlg.show();
        updateDlg.exec();
        this->updateUIElements();
    } else {
        QMessageBox::warning(this,
                             tr("Nothing to do"),
                             tr("All database entries are up to date!"),
                             QMessageBox::Ok);
    }
}

void EditDatabaseDlg::onDelEntryButtonPressed()
{
    if ((ui->fileList->currentRow() >= 0)
        && (ui->fileList->currentRow() < this->m_database->size())) {
        this->m_database->removeEntry(ui->fileList->currentRow());
        this->updateUIElements();
    }
}

void EditDatabaseDlg::updateUIElements()
{
    ui->databaseInfoLabel->setText(
            tr("%1 files in database \"%2\"; %3 do not exist or are untracked yet.").arg(
                    QString::number(this->m_database->size()),
                    this->m_databaseFile,
                    QString::number(this->m_database->filesNotUpToDate())));
    ui->fileList->clear();
    for (int i = 0; i < this->m_database->size(); i++) {
        ui->fileList->addItem(this->m_database->pictureAt(i)->getFile());
    }
    this->onFileSelected(ui->fileList->currentRow());
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
                ui->processedLabel->setText(tr("The average color is (%1, %2, %3).").arg(
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
        ui->processedLabel->clear();
    }
}

void EditDatabaseDlg::onAddDirButtonPressed()
{
    QString dirToAdd = QFileDialog::getExistingDirectory(
            this,
            tr("Select picture folder"),
            QDir::homePath(),
            QFileDialog::ShowDirsOnly);
    if (dirToAdd.isNull() || (dirToAdd.length() == 0)) {
        return;
    }
    IndexDirDlg indexDirDlg(this,
                            this->m_database,
                            dirToAdd
                            );
    indexDirDlg.show();
    indexDirDlg.exec();
    if (!this->m_database->allUpToDate()) {
        UpdateDatabaseDlg updateDlg(this, this->m_database);
        updateDlg.show();
        updateDlg.exec();
    }
    this->updateUIElements();
}

void EditDatabaseDlg::onAddFileButtonPressed()
{
    QString filename
            = QFileDialog::getOpenFileName(this,
                                           tr("Select image file"),
                                           QDir::homePath(),
                                           tr("Images (*.png *.bmp *.xpm *.jpg);;All files (*.*)"));
    if (filename.isNull() || (filename.length() == 0)) {
        return;
    }
    this->m_database->indexFile(filename);
    if (!this->m_database->allUpToDate()) {
        UpdateDatabaseDlg updateDlg(this, this->m_database);
        updateDlg.show();
        updateDlg.exec();
    }
    this->updateUIElements();
}

void EditDatabaseDlg::onCloseButtonPressed()
{
    if (QMessageBox::question(this,
                              tr("Save changes?"),
                              tr("Do you want to save the changes you made to the %1 \"%2\"?").arg(
                                      "database", this->m_databaseFile),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (!this->m_database->toFile(this->m_databaseFile)) {
            QMessageBox::warning(this,
                                 tr("Error!"),
                                 tr("Could not write the file \"%1\"!").arg(
                                         this->m_databaseFile),
                                 QMessageBox::Ok);
            return;
        }
    }
    done(0);
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
