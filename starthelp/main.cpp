/***************************************************************************************************
*
* FILE:     main.cpp
*
* CREATED:  12-09-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  starts the assistant with the right collection file
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#include <QtCore/QCoreApplication>
#include <QLocale>
#include <QString>
#include <QFile>
#include <QProcess>
#include <QDebug>

#define HELP_DIR            "help/"
#define HELPFILENAME_BASE   "doc_"
#define HELPFALLBACKFILE    "docengl.qhc"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString helpFile = a.applicationDirPath();
    helpFile += "/";
    helpFile += HELP_DIR;
    helpFile += HELPFILENAME_BASE;
    QString isoLocale;
    switch (QLocale::system().language()) {
    case QLocale::German:
        isoLocale = "de";
        break;
    default:
        isoLocale = "en";
        break;
    }

    helpFile += isoLocale;
    helpFile += ".qhc";

    //qDebug() << helpFile;

    if (!QFile::exists(helpFile)) {
        helpFile = a.applicationDirPath() + "/" + HELP_DIR + HELPFALLBACKFILE;
    }

    QString command;
    QString args;
#ifdef Q_OS_WIN32
    command = QString("\"") + a.applicationDirPath() + "/assistant.exe" + QString("\"");
    args =  " -collectionFile " + QString("\"") + helpFile + "\"";
#endif
#ifdef Q_OS_LINUX
    command = "\"" + a.applicationDirPath() + "/assistant" + QString("\"");
    args = " -collectionFile" + QString("\"") + helpFile + "\"";
#endif

    command += args;
    QProcess::startDetached(command);

    //return a.exec();
}
