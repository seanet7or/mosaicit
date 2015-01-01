/***************************************************************************************************
*
* FILE:     debug.cpp
*
* CREATED:  23-11-2013
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  allows to log debug messages and show errors and warnings
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010-2013 by Benjamin Caspari
*
***************************************************************************************************/
#include "debug.h"
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QStack>
#include "window.h"

void Debug::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

bool Debug::init()
{
    //qInstallMessageHandler(Debug::messageHandler);
	return true;
}
