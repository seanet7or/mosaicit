/***************************************************************************************************
*
* FILE:     appsettings.h
*
* CREATED:  03-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  singleton to store application-wide settings
*
* This program is licensed under the terms of the GPL Version 2
*
* Copyright 2010 by Benjamin Caspari
*
***************************************************************************************************/

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>


#define DEF_ORGANIZATION			"mosaicit.de"
#define DEF_ORGANIZATION_DOMAIN		"mosaicit.de"
#define DEF_APPLICATION_NAME		"MosaicIt"
#define DEF_APPLICATION_ICON		":/icon"
#define DEF_LOGFILE					"log.txt"

#define DEF_WINDOW_SIZE				800, 600


class AppSettings
{
public:
	static bool init();
    static AppSettings &getInstance();
	static const QString &settingsDirectory();
	
signals:

public slots:

private:
    explicit AppSettings();
	QString m_settingsDirectory;
};

#endif // APPSETTINGS_H
