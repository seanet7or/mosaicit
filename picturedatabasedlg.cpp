#include "picturedatabasedlg.h"
#include "ui_picturedatabasedlg.h"
#include <QPicture>
#include "picturelibrary.h"

PictureDatabaseDlg::PictureDatabaseDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PictureDatabaseDlg)
{
    ui->setupUi(this);

    m_picturesByDir = m_library.picturesByDirectory();

    foreach (QString dir, m_picturesByDir.keys()) {
        QListWidgetItem *currentDir = new QListWidgetItem(dir, ui->directoryList);
        currentDir->setFlags(currentDir->flags() | Qt::ItemIsUserCheckable);
        bool enabledPictures = false;
        bool disabledPictures = false;
        foreach (PictureInfo *p, m_picturesByDir[dir]) {
            if (p->enabled()) {
                enabledPictures = true;
            } else {
                disabledPictures = true;
            }
        }
        if (enabledPictures && (!disabledPictures)) {
            currentDir->setCheckState(Qt::Checked);
        } else if (disabledPictures && (!enabledPictures)) {
            currentDir->setCheckState(Qt::Unchecked);
        } else if (disabledPictures && enabledPictures) {
            currentDir->setCheckState(Qt::PartiallyChecked);
        }
    }

    connect(ui->directoryList,
            &QListWidget::itemSelectionChanged,
            this,
            &PictureDatabaseDlg::onDirectorySelected);
    connect(ui->fileList,
            &QListWidget::itemSelectionChanged,
            this,
            &PictureDatabaseDlg::onFileSelected);
}

PictureDatabaseDlg::~PictureDatabaseDlg()
{
    delete ui;
}

void PictureDatabaseDlg::onDirectorySelected()
{
    int pictures = 0;
    ui->fileList->clear();
    QList<QListWidgetItem*> selectedDirs = ui->directoryList->selectedItems();
    foreach (QListWidgetItem *selectedDir, selectedDirs) {
        QString dir = selectedDir->text();
        QVector<PictureInfo*> picturesInCurrentDir = m_picturesByDir[dir];
        foreach (PictureInfo *p, picturesInCurrentDir) {
            QListWidgetItem *currentFile = new QListWidgetItem(p->filename(), ui->fileList);
            currentFile->setToolTip(p->getPath());
            currentFile->setFlags(currentFile->flags() | Qt::ItemIsUserCheckable);
            if (p->enabled()) {
                currentFile->setCheckState(Qt::Checked);
            } else {
                currentFile->setCheckState(Qt::Unchecked);
            }
            pictures++;
        }
    }
    ui->fileCountLabel->setText(tr("%1 pictures").arg(QString::number(pictures)));
}

void PictureDatabaseDlg::onFileSelected()
{
    QList<QListWidgetItem*> selectedFiles = ui->fileList->selectedItems();
    if (selectedFiles.length() > 0) {
        QListWidgetItem *selectedFile = selectedFiles.first();
        ui->imagePreview->setPixmap(QPixmap(selectedFile->toolTip()));
    } else {
        ui->imagePreview->clear();
    }
}
