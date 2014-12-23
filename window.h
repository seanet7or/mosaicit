/***************************************************************************************************
*
* FILE:     window.h
*
* CREATED:  24-11-2013
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  base class for applications windows
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2013 by Benjamin Caspari
*
***************************************************************************************************/
#ifndef WINDOW_H
#define WINDOW_H
#include <QDialog>
#include <QDebug>
#include "appsettings.h"


class Window : public QDialog
{
public:
	Window(const QString &title,
		const QString &windowID = "defaultWin",
		QSize defaultSize = QSize(DEF_WINDOW_SIZE));
};

#endif // WINDOW_H
