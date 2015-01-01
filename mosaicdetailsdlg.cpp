/***************************************************************************************************
*
* FILE:     mosaicdetailsdlg.cpp
*
* CREATED:  13-05-2010
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  dialog to set detailed options for a mosaic
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#include "mosaicdetailsdlg.h"
#include "ui_mosaicdetailsdlg.h"

#include <QCloseEvent>

#include "appsettings.h"

MosaicDetailsDlg::MosaicDetailsDlg(QWidget *parent, const QString &imageFile) :
        QDialog(parent),
        ui(new Ui::MosaicDetailsDlg)
{
    ui->setupUi(this);
    ui->renderBn->setText(tr("Render"));
    ui->renderBn->setHorizontalAlignment(Qt::AlignHCenter);

    setTabOrder(ui->aspect11, ui->aspect43);
    setTabOrder(ui->aspect43, ui->aspect169);
    setTabOrder(ui->cutTileEdges, ui->scaleTile);
    setTabOrder(ui->minDistanceChecker, ui->minDistanceSpinner);
    setTabOrder(ui->minDistanceSpinner, ui->repeatTilesChecker);
    setTabOrder(ui->repeatTilesChecker, ui->repeatTilesSpinner);
    setTabOrder(ui->repeatTilesSpinner, ui->alphaChannel);
    setTabOrder(ui->alphaChannel, ui->renderBn);

    this->m_canceled = true;
    m_image = QImage(imageFile);
    if (m_image.isNull()) {
        ui->previewImage->clear();
        return;
    } else {
        int lw = ui->previewImage->width();
        int lh = ui->previewImage->height();
        ui->previewImage->setPixmap(
                QPixmap::fromImage(
                        m_image.scaled(QSize(lw, lh),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation)
                        )
                    );
    }
    connect(ui->renderBn,
            &Button::pressed,
            this,
            &MosaicDetailsDlg::renderButtonPressed);
    connect(ui->tileSizeSlider,
            &Slider::valueChanged,
            this,
            &MosaicDetailsDlg::tileWidthChanged);
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
    connect(ui->totalTilesSlider,
            &Slider::valueChanged,
            this,
            &MosaicDetailsDlg::totalTilesCountChanged);
    this->readSettings();
}

bool MosaicDetailsDlg::exitedCorrectly()
{
    return !this->m_canceled;
}

void MosaicDetailsDlg::updateResultLabels()
{
    ui->tileCountLabel->setText(tr("The mosaic will be made of %1 pieces").arg(QString::number(tileCount())));
    ui->tileSizeLabel->setText(tr("%1 x %2 pixels").arg(QString::number(tileWidth()), QString::number(tileHeight())));
    ui->resultLabel->setText(
            tr("The resulting mosaic image will have a size of %1 megapixels.").arg(
                    QString::number((float)ui->totalTilesSlider->value()
                                    * (float)tileWidth()
                                    * (float)tileHeight()
                                    / 1000000.f)));
}

void MosaicDetailsDlg::tileWidthChanged(int)
{  
    this->updateResultLabels();
}

void MosaicDetailsDlg::totalTilesCountChanged(int)
{
    this->updateResultLabels();
}

void MosaicDetailsDlg::aspectRatioChanged()
{
    this->updateResultLabels();
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

int MosaicDetailsDlg::tileWidth() const
{
    return ui->tileSizeSlider->value();
}

int MosaicDetailsDlg::tileHeight() const
{
    if (ui->aspect11->isChecked()) {
        return tileWidth();
    } else if (ui->aspect169->isChecked()) {
        return ((tileWidth() * 9) / 16);
    } else if (ui->aspect43->isChecked()) {
        return ((tileWidth() * 3) / 4);
    }
    return tileWidth();
}

int MosaicDetailsDlg::tileCount() const
{
    return ui->totalTilesSlider->value();
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
    QSettings settings;
    settings.beginGroup("GUIStateMosaicDetailsDlg");
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.endGroup();
    settings.beginGroup("InputMosaicDetailsDlg");
    settings.setValue("tilewidth", tileWidth());
    int tileAspectRatioSelection = 3;
    if (ui->aspect11->isChecked()) {
        tileAspectRatioSelection = 1;
    } else if (ui->aspect169->isChecked()) {
        tileAspectRatioSelection = 2;
    } else if (ui->aspect43->isChecked()) {
        tileAspectRatioSelection = 3;
    }
    settings.setValue("tileaspectratioselection", tileAspectRatioSelection);
    settings.setValue("cutedges", this->m_cutEdges);
    settings.setValue("totaltilenumber", tileCount());
    settings.setValue("alphachannel", this->m_alphaChannel);
    settings.setValue("mindistancechecker", this->m_minDistanceChecker);
    settings.setValue("mindistancespinner", this->m_minDistance);
    settings.setValue("repeattileschecker", this->m_repeatTilesChecker);
    settings.setValue("repeattilesspinner", this->m_repeatTilesMax);
    settings.endGroup();
}

void MosaicDetailsDlg::updateValuesFromForms()
{
    this->m_alphaChannel = ui->alphaChannel->value();
    this->m_cutEdges = ui->cutTileEdges->isChecked();
    this->m_minDistanceChecker = ui->minDistanceChecker->isChecked();
    this->m_minDistance = ui->minDistanceSpinner->value();
    this->m_repeatTilesChecker = ui->repeatTilesChecker->isChecked();
    this->m_repeatTilesMax = ui->repeatTilesSpinner->value();
}

void MosaicDetailsDlg::readSettings()
{
    QSettings settings;
    settings.beginGroup("GUIStateMosaicDetailsDlg");
    this->resize(settings.value("size", QSize(620, 355)).toSize());
    this->move(settings.value("pos", QPoint(180, 175)).toPoint());
    settings.endGroup();
    settings.beginGroup("InputMosaicDetailsDlg");
    ui->tileSizeSlider->setValue(settings.value("tilewidth", 128).toInt());
    int tileAspectRatioSelection = settings.value("tileaspectratioselection", 3).toInt();
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
    default:
        ui->aspect43->setChecked(true);
    }
    if (settings.value("cutedges", true).toBool()) {
        ui->cutTileEdges->setChecked(true);
    } else {
        ui->scaleTile->setChecked(true);
    }
    ui->totalTilesSlider->setValue(settings.value("totaltilenumber", 8000).toInt());
    ui->alphaChannel->setValue(settings.value("alphachannel", 30).toInt());
    ui->minDistanceChecker->setChecked(settings.value("mindistancechecker", true).toBool());
    ui->minDistanceSpinner->setValue(settings.value("mindistancespinner", 3).toInt());
    ui->repeatTilesChecker->setChecked(settings.value("repeattileschecker", false).toBool());
    ui->repeatTilesSpinner->setValue(settings.value("repeattilesspinner", 300).toInt());
    settings.endGroup();
    this->updateResultLabels();
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
