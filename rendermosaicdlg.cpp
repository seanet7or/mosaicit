#include "rendermosaicdlg.h"
#include "ui_rendermosaicdlg.h"

#include <QMessageBox>
#include <QDir>

#include "rendermosaicthread.h"
#include "picturedatabase.h"
#include "appsettings.h"

RenderMosaicDlg::RenderMosaicDlg(QWidget *parent,
                                 PictureDatabase *database,
                                 const QString &imageFile,
                                 int tileWidth,
                                 int tileHeight,
                                 int tileCount,
                                 bool cutEdges,
                                 int alphaChannel,
                                 const QString &outputFile,
                                 bool minDistChecker,
                                 int minDist,
                                 bool maxTileRepeatChecker,
                                 int maxTileRepeatCount) :
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
                                       outputFile,
                                       this,
                                       minDistChecker,
                                       minDist,
                                       maxTileRepeatChecker,
                                       maxTileRepeatCount);
    this->readSettings();
}

RenderMosaicDlg::~RenderMosaicDlg()
{
    this->writeSettings();
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
            this->writeSettings();
            done(0);
        }
    } else {
        this->writeSettings();
        done(0);
    }
}

void RenderMosaicDlg::reject()
{
    this->cancelBnPressed();
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
                this->writeSettings();
                e->accept();
            } else {
                e->ignore();
            }
        } else {
            this->writeSettings();
            e->accept();
        }
    } else {
        this->writeSettings();
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
            ui->label->setText(tr("The mosaic was built and saved to %1.").arg(
                    QDir::toNativeSeparators(QDir::cleanPath(this->m_renderThread->outputFile()))));
        }
    }
    ui->cancelButton->setText(tr("Close"));
}

void RenderMosaicDlg::writeSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateRenderMosaicDlg");
    settings->setValue("size", this->size());
    settings->setValue("pos", this->pos());
    settings->endGroup();
}

void RenderMosaicDlg::readSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateRenderMosaicDlg");
    this->resize(settings->value("size", QSize(412, 274)).toSize());
    this->move(settings->value("pos", QPoint(265, 215)).toPoint());
    settings->endGroup();
}
