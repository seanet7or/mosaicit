/***************************************************************************************************
*
* FILE:     settings.h
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
#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>


class Settings
{
public:
    static QString configDirectory();

    Settings();
    ~Settings();
};

#endif // SETTINGS_H
