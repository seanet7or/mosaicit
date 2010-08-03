/***************************************************************************************************
*
* FILE:     appsettings.cpp
*
* CREATED:  03-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  Sigleton class to store and load application wide settings
*
* Licensed under the GPL
*
***************************************************************************************************/

#include "appsettings.h"

AppSettings *AppSettings::getInstance()
{
    static AppSettings *instance = 0;
    if (!instance) {
        instance = new AppSettings;
    }
    return instance;
}

QSettings *AppSettings::settings()
{
    return AppSettings::getInstance()->m_settings;
}

AppSettings::AppSettings(QObject *parent) :
    QObject(parent)
{
    this->m_settings = new QSettings(QSettings::IniFormat,
                                     QSettings::UserScope,
                                     "becaspari",
                                     "mosaicit");
}
