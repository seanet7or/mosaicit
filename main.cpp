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
#include "mainwindow.h"
#include "debug.h"
#include "appsettings.h"
#include "database_workers/databaseworkmanager.h"

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
	if (!Debug::init())
	{
		return 3;
	}
    if (!PictureDatabase::openDb())
    {
        return 4;
    }

	qDebug() << "Starting up.";
    DatabaseWorkManager workManager;
    workManager.startWork();
    MainWindow w;
    w.show();
    int res = a.exec();
    workManager.requestCancel();
    return res;
}
