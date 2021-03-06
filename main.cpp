/***************************************************************************************************
*
* FILE:     main.cpp
*
* CREATED:  10-05-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  the program entry point
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>

#include "mainwindow.h"
#include "appsettings.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
	 QTranslator myappTranslator;
    myappTranslator.load(a.applicationDirPath() + "/mosaicit_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

	if (!AppSettings::init())
	{
		return 2;
	}
	/*if (!Debug::init())
	{
		return 3;
	}*/

	qDebug() << "Starting up.";

    MainWindow w;
    w.show();
    return a.exec();
}
