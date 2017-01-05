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
#include "window.h"
#include <QIcon>
#include "appsettings.h"


Window::Window(const QString &title,
        const QString & /*windowID*/,
        QSize /*defaultSize*/)
	: QDialog(NULL, Qt::Window)
{
	setWindowIcon(QIcon(DEF_APPLICATION_ICON));
	setWindowTitle(title);
}
