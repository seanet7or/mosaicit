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
#include <QPointer>
class QTreeWidget;
class Window;


class Debug
{
public:
	static bool init();

private:
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
#ifdef QT_DEBUG
	static QPointer<Window> m_debugWindow;
	static QPointer<QTreeWidget> m_debugLogWidget;
#endif
};

#endif // DEBUG_H
