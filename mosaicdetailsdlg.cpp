#include "mosaicdetailsdlg.h"
#include "ui_mosaicdetailsdlg.h"

MosaicDetailsDlg::MosaicDetailsDlg(QWidget *parent, const QString &imageFile) :
        QDialog(parent),
        ui(new Ui::MosaicDetailsDlg)
{
    ui->setupUi(this);
    m_image = QImage(imageFile);
    if (m_image.isNull()) {
        ui->previewImage->clear();
        return;
    } else {
        int lw = ui->previewImage->width();
        int lh = ui->previewImage->height();
        ui->previewImage->setPixmap(
                QPixmap::fromImage(m_image.scaled(QSize(lw, lh),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation)));
    }
    ui->tileWidth->setValue(300);
    ui->tileHeight->setValue(300);
    ui->aspect11->setChecked(true);
    ui->cutTileEdges->setChecked(true);
    ui->totalTiles->setValue(200);
    ui->alphaChannel->setValue(30);
    this->updateResultLabel();
    connect(ui->tileHeight,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(tileHeightChanged()));
    connect(ui->tileWidth,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(tileWidthChanged()));
    connect(ui->aspect11,
            SIGNAL(toggled(bool)),
            this,
            SLOT(aspectRatioChanged()));
    connect(ui->aspect169,
            SIGNAL(toggled(bool)),
            this,
            SLOT(aspectRatioChanged()));
    connect(ui->aspect43,
            SIGNAL(toggled(bool)),
            this,
            SLOT(aspectRatioChanged()));
    connect(ui->aspectFree,
            SIGNAL(toggled(bool)),
            this,
            SLOT(aspectRatioChanged()));
    connect(ui->totalTiles,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(updateResultLabel()));
}

void MosaicDetailsDlg::updateResultLabel()
{
    ui->resultLabel->setText(
            tr("The resulting image will have a size of %1 megapixels.").arg(
                    QString::number((float)ui->totalTiles->value()
                                    * (float)ui->tileWidth->value()
                                    * (float)ui->tileHeight->value()
                                    / 1000000.f)));
}

void MosaicDetailsDlg::tileWidthChanged()
{
    ui->tileHeight->disconnect();
    if (ui->aspect11->isChecked()) {
        ui->tileHeight->setValue(ui->tileWidth->value());
    } else if (ui->aspect169->isChecked()) {
        ui->tileHeight->setValue((ui->tileWidth->value() * 9) / 16);
    } else if (ui->aspect43->isChecked()) {
        ui->tileHeight->setValue((ui->tileWidth->value() * 3) / 4);
    }
    connect(ui->tileHeight,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(tileHeightChanged()));
    this->updateResultLabel();
}

void MosaicDetailsDlg::tileHeightChanged()
{
    ui->tileWidth->disconnect();
    if (ui->aspect11->isChecked()) {
        ui->tileWidth->setValue(ui->tileHeight->value());
    } else if (ui->aspect169->isChecked()) {
        ui->tileWidth->setValue((ui->tileHeight->value() * 16) / 9);
    } else if (ui->aspect43->isChecked()) {
        ui->tileWidth->setValue((ui->tileHeight->value() * 4) / 3);
    }
    connect(ui->tileWidth,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(tileWidthChanged()));
    this->updateResultLabel();
}

void MosaicDetailsDlg::aspectRatioChanged()
{
    ui->tileHeight->disconnect();
    if (ui->aspect11->isChecked()) {
        ui->tileHeight->setValue(ui->tileWidth->value());
    } else if (ui->aspect169->isChecked()) {
        ui->tileHeight->setValue((ui->tileWidth->value() * 9) / 16);
    } else if (ui->aspect43->isChecked()) {
        ui->tileHeight->setValue((ui->tileWidth->value() * 3) / 4);
    }
    connect(ui->tileHeight,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(tileHeightChanged()));
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
