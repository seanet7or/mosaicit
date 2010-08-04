#include "mosaicdetailsdlg.h"
#include "ui_mosaicdetailsdlg.h"

#include <QCloseEvent>

#include "appsettings.h"

MosaicDetailsDlg::MosaicDetailsDlg(QWidget *parent, const QString &imageFile) :
        QDialog(parent),
        ui(new Ui::MosaicDetailsDlg)
{
    ui->setupUi(this);
    this->m_canceled = true;
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
    connect(ui->cancelButton,
            SIGNAL(pressed()),
            this,
            SLOT(cancelButtonPressed()));
    connect(ui->renderButton,
            SIGNAL(pressed()),
            this,
            SLOT(renderButtonPressed()));
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
    this->readSettings();
}

bool MosaicDetailsDlg::exitedCorrectly()
{
    return !this->m_canceled;
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
    this->writeSettings();
    delete ui;
}

void MosaicDetailsDlg::cancelButtonPressed()
{
    this->writeSettings();
    done(0);
}

void MosaicDetailsDlg::renderButtonPressed()
{
    this->m_canceled = false;
    this->updateValuesFromForms();
    done(0);
}

int MosaicDetailsDlg::tileWidth()
{
    return this->m_tileWidth;
}

int MosaicDetailsDlg::tileHeight()
{
    return this->m_tileHeight;
}

int MosaicDetailsDlg::tileCount()
{
    return this->m_tileCount;
}

bool MosaicDetailsDlg::cutEdges()
{
    return this->m_cutEdges;
}

int MosaicDetailsDlg::alphaChannel()
{
    return this->m_alphaChannel;
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

void MosaicDetailsDlg::closeEvent(QCloseEvent *e)
{
    this->writeSettings();
    e->accept();
}

void MosaicDetailsDlg::writeSettings()
{
    this->updateValuesFromForms();
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateMosaicDetailsDlg");
    settings->setValue("size", this->size());
    settings->setValue("pos", this->pos());
    settings->endGroup();
    settings->beginGroup("InputMosaicDetailsDlg");
    settings->setValue("tilewidth", this->m_tileWidth);
    settings->setValue("tileheight", this->m_tileHeight);
    int tileAspectRatioSelection = 3;
    if (ui->aspect11->isChecked()) {
        tileAspectRatioSelection = 1;
    } else if (ui->aspect169->isChecked()) {
        tileAspectRatioSelection = 2;
    } else if (ui->aspect43->isChecked()) {
        tileAspectRatioSelection = 3;
    } else if (ui->aspectFree->isChecked()) {
        tileAspectRatioSelection = 4;
    }
    settings->setValue("tileaspectratioselection", tileAspectRatioSelection);
    settings->setValue("cutedges", this->m_cutEdges);
    settings->setValue("totaltilenumber", this->m_tileCount);
    settings->setValue("alphachannel", this->m_alphaChannel);
    settings->setValue("mindistancechecker", this->m_minDistanceChecker);
    settings->setValue("mindistancespinner", this->m_minDistance);
    settings->setValue("repeattileschecker", this->m_repeatTilesChecker);
    settings->setValue("repeattilesspinner", this->m_repeatTilesMax);
    settings->endGroup();
}

void MosaicDetailsDlg::updateValuesFromForms()
{
    this->m_alphaChannel = ui->alphaChannel->value();
    this->m_cutEdges = ui->cutTileEdges->isChecked();
    this->m_tileCount = ui->totalTiles->value();
    this->m_tileHeight = ui->tileHeight->value();
    this->m_tileWidth = ui->tileWidth->value();
    this->m_minDistanceChecker = ui->minDistanceChecker->isChecked();
    this->m_minDistance = ui->minDistanceSpinner->value();
    this->m_repeatTilesChecker = ui->repeatTilesChecker->isChecked();
    this->m_repeatTilesMax = ui->repeatTilesSpinner->value();
}

void MosaicDetailsDlg::readSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateMosaicDetailsDlg");
    this->resize(settings->value("size", QSize(620, 355)).toSize());
    this->move(settings->value("pos", QPoint(180, 175)).toPoint());
    settings->endGroup();
    settings->beginGroup("InputMosaicDetailsDlg");
    ui->tileWidth->setValue(settings->value("tilewidth", 150).toInt());
    ui->tileHeight->setValue(settings->value("tileheight", 150).toInt());
    int tileAspectRatioSelection = settings->value("tileaspectratioselection", 3).toInt();
    switch (tileAspectRatioSelection) {
    case 1:
        ui->aspect11->setChecked(true);
        break;
    case 2:
        ui->aspect169->setChecked(true);
        break;
    case 3:
        ui->aspect43->setChecked(true);
        break;
    case 4:
        ui->aspectFree->setChecked(true);
        break;
    default:
        ui->aspect43->setChecked(true);
    }
    if (settings->value("cutedges", true).toBool()) {
        ui->cutTileEdges->setChecked(true);
    } else {
        ui->scaleTile->setChecked(true);
    }
    ui->totalTiles->setValue(settings->value("totaltilenumber", 800).toInt());
    ui->alphaChannel->setValue(settings->value("alphachannel", 30).toInt());
    ui->minDistanceChecker->setChecked(settings->value("mindistancechecker", true).toBool());
    ui->minDistanceSpinner->setValue(settings->value("mindistancespinner", 3).toInt());
    ui->repeatTilesChecker->setChecked(settings->value("repeattileschecker", false).toBool());
    ui->repeatTilesSpinner->setValue(settings->value("repeattilesspinner", 300).toInt());
    settings->endGroup();
    this->updateResultLabel();
}

bool MosaicDetailsDlg::minDistanceChecker()
{
    return this->m_minDistanceChecker;
}

int MosaicDetailsDlg::minDistance()
{
    return this->m_minDistance;
}

bool MosaicDetailsDlg::repeatTilesMaxChecker()
{
    return this->m_repeatTilesChecker;
}

int MosaicDetailsDlg::repeatTilesMaxCount()
{
    return this->m_repeatTilesMax;
}
