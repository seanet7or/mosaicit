/***************************************************************************************************
*
* FILE:     aboutdlg.cpp
*
* CREATED:  02-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  Header file for the about dialog
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    connect(ui->pushButton,
            SIGNAL(pressed()),
            this,
            SLOT(onOkButtonPressed()));
    this->readSettings();
}

AboutDlg::~AboutDlg()
{
    this->writeSettings();
    delete ui;
}

void AboutDlg::changeEvent(QEvent *e)
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

void AboutDlg::onOkButtonPressed()
{
    this->writeSettings();
    done(0);
}

void AboutDlg::writeSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateAboutDlg");
    settings->setValue("size", this->size());
    settings->setValue("pos", this->pos());
    settings->endGroup();
}

void AboutDlg::readSettings()
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("GUIStateAboutDlg");
    this->resize(settings->value("size", QSize(620, 400)).toSize());
    this->move(settings->value("pos", QPoint(110, 170)).toPoint());
    settings->endGroup();
}
