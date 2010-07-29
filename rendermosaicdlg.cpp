#include "rendermosaicdlg.h"
#include "ui_rendermosaicdlg.h"

#include "rendermosaicthread.h"
#include "picturedatabase.h"

RenderMosaicDlg::RenderMosaicDlg(QWidget *parent,
                                 PictureDatabase *database,
                                 const QString &imageFile,
                                 int tileWidth,
                                 int tileHeight,
                                 int tileCount,
                                 bool cutEdges,
                                 int alphaChannel) :
    QDialog(parent),
    ui(new Ui::RenderMosaicDlg)
{
    ui->setupUi(this);
    this->m_renderThread = new RenderMosaicThread;
    connect(this->m_renderThread,
            SIGNAL(logText(QString)),
            this,
            SLOT(logText(QString)));
    this->m_renderThread->renderMosaic(database,
                                       imageFile,
                                       tileWidth,
                                       tileHeight,
                                       tileCount,
                                       cutEdges,
                                       alphaChannel);
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
