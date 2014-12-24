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

#ifdef QT_DEBUG
QPointer<Window> Debug::m_debugWindow = NULL;
QPointer<QTreeWidget> Debug::m_debugLogWidget = NULL;



void Debug::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QStack<QTreeWidgetItem*> parentWidget;
	static QStack<QString> parentContext;

	QTreeWidgetItem *newItem;
	if (parentWidget.isEmpty())
	{
		newItem = new QTreeWidgetItem(m_debugLogWidget);
		parentWidget.push(newItem);
		parentContext.push(QString(context.function));
	}
	else
	{
		if (QString(context.function) == parentContext.top())
		{
			newItem = new QTreeWidgetItem(parentWidget.top()->parent());
			parentWidget.push(newItem);
			parentContext.push(QString(context.function));
		}
		else if ((parentContext.size() > 1) 
			&& (parentContext.at(parentContext.size() - 2) == QString(context.function)))
		{
			parentWidget.pop();
			parentContext.pop();
			newItem = new QTreeWidgetItem(parentWidget.top());
		}
		else
		{
			newItem = new QTreeWidgetItem(parentWidget.top());
			parentWidget.push(newItem);
			parentContext.push(QString(context.function));
		}
	}
	newItem->setText(0, msg);

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
#endif

bool Debug::init()
{
#ifdef QT_DEBUG
	if (!m_debugWindow)
	{
		m_debugWindow = new Window(QObject::tr("Debug"));
		QVBoxLayout *layout = new QVBoxLayout(m_debugWindow);
		m_debugWindow->setLayout(layout);
		m_debugLogWidget = new QTreeWidget(m_debugWindow);
		layout->addWidget(m_debugLogWidget);
	}
	m_debugWindow->show();
#endif

    //qInstallMessageHandler(Debug::messageHandler);
	return true;
}
