/***************************************************************************************************
*
* FILE:     settings.cpp
*
* CREATED:  12-23-2014
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  application-wide settings
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright by Benjamin Caspari
*
***************************************************************************************************/
#include "settings.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Settings::Settings()
{
}

Settings::~Settings()
{
}

QString Settings::configDirectory()
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QDir dir(directory);
    if (!dir.mkpath(directory))
    {
        qCritical() << "Failed to create directory "
            << QDir::toNativeSeparators(directory);
        return QString::null;
    }
    return directory;
}
