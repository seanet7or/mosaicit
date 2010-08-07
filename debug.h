/***************************************************************************************************
*
* FILE:     debug.h
*
* CREATED:  10-05-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  allows to print debug messages to the console
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <QString>
#include <QDebug>

class Debug
{
public:
    static void log(QString text)
    {
        qDebug() << text;
    }
};

#endif // DEBUG_H
