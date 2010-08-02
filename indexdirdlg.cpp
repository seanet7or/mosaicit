#include "indexdirdlg.h"
#include "ui_indexdirdlg.h"

#include <QMessageBox>

IndexDirDlg::IndexDirDlg(QWidget *parent,
                         PictureDatabase *database,
                         const QString &newDir) :
    QDialog(parent),
    ui(new Ui::IndexDirDlg)
{
    ui->setupUi(this);
    this->m_database = database;
    bool subDirs = false;
    if (QMessageBox::question(this,
                              tr("Include subdirectories?"),
                              tr("Do you also want to add the image files in the subdirectories?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        subDirs = true;
    } else {
        subDirs = false;
    }
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(onCancelButtonPressed()));
    connect(database,
            SIGNAL(indexFinished()),
            this,
            SLOT(onIndexingFinished()));
    database->indexFiles(newDir, subDirs);
}

IndexDirDlg::~IndexDirDlg()
{
    delete ui;
}

void IndexDirDlg::onCancelButtonPressed()
{
    if (this->m_database->isIndexingRunning()) {
        if (QMessageBox::question(this,
                                  tr("Cancel indexing?"),
                                  tr("Do you want to cancel? Not all files in the selected %1").arg(
                                          "directory will be added!"),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            this->m_database->cancelIndexing();
        }
    }
}

void IndexDirDlg::onIndexingFinished()
{
    done(0);
}

void IndexDirDlg::changeEvent(QEvent *e)
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
