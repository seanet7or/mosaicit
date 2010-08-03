#include "rendermosaicdlg.h"
#include "ui_rendermosaicdlg.h"

#include <QMessageBox>

#include "rendermosaicthread.h"
#include "picturedatabase.h"

RenderMosaicDlg::RenderMosaicDlg(QWidget *parent,
                                 PictureDatabase *database,
                                 const QString &imageFile,
                                 int tileWidth,
                                 int tileHeight,
                                 int tileCount,
                                 bool cutEdges,
                                 int alphaChannel,
                                 const QString &outputFile) :
QDialog(parent),
ui(new Ui::RenderMosaicDlg)
{
    ui->setupUi(this);
    this->m_outputFile = outputFile;
    this->m_renderThread = new RenderMosaicThread;
    connect(this->m_renderThread,
            SIGNAL(logText(QString)),
            this,
            SLOT(logText(QString)));
    connect(this->m_renderThread,
            SIGNAL(renderComplete(float)),
            this,
            SLOT(renderComplete(float)));
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(cancelBnPressed()));
    connect(this->m_renderThread,
            SIGNAL(finished()),
            this,
            SLOT(renderThreadFinished()));
    this->m_renderThread->renderMosaic(database,
                                       imageFile,
                                       tileWidth,
                                       tileHeight,
                                       tileCount,
                                       cutEdges,
                                       alphaChannel,
                                       outputFile);
}

RenderMosaicDlg::~RenderMosaicDlg()
{
    delete ui;
    if (this->m_renderThread) {
        while (this->m_renderThread->isRunning()) {
            this->m_renderThread->cancel();
        }
        delete this->m_renderThread;
        this->m_renderThread = 0;
    }
}

void RenderMosaicDlg::changeEvent(QEvent *e)
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

void RenderMosaicDlg::logText(const QString &text)
{
    ui->textLog->appendPlainText(text);
}

void RenderMosaicDlg::renderComplete(float percent)
{
    ui->progressBar->setValue((int)percent);
}

void RenderMosaicDlg::cancelBnPressed()
{
    if (this->m_renderThread) {
        if (this->m_renderThread->isRunning()) {
            if (QMessageBox::question(this,
                                      tr("Do you want to cancel?"),
                                      tr("Do you want to cancel the build?"),
                                      QMessageBox::No | QMessageBox::Yes
                                      ) == QMessageBox::Yes) {
                while (this->m_renderThread->isRunning()) {
                    this->m_renderThread->cancel();
                }
            }
        } else {
            done(0);
        }
    }
}

void RenderMosaicDlg::closeEvent(QCloseEvent *e)
{
    if (this->m_renderThread) {
        if (this->m_renderThread->isRunning()) {
            if (QMessageBox::question(this,
                                      tr("Do you want to cancel?"),
                                      tr("Do you want to cancel the build?"),
                                      QMessageBox::No | QMessageBox::Yes
                                      ) == QMessageBox::Yes) {
                while (this->m_renderThread->isRunning()) {
                    this->m_renderThread->cancel();
                }
                e->accept();
            } else {
                e->ignore();
            }
        } else {
            e->accept();
        }
    } else {
        e->accept();
    }
}

void RenderMosaicDlg::renderThreadFinished()
{
    if (this->m_renderThread) {
        disconnect(this->m_renderThread,
                   SIGNAL(finished()));
        if (this->m_renderThread->wasCanceled()) {
            ui->label->setText(tr("The process was canceled, no mosaic was saved!"));
        } else if (this->m_renderThread->criticalError()) {
            ui->label->setText(tr("A critical error occured, view detailed output!"));
        } else {
            ui->label->setText(tr("The mosaic was built and saved to %1.").arg(this->m_outputFile));
        }
    }
    ui->cancelButton->setText(tr("Close"));
}
