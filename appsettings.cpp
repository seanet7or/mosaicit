/***************************************************************************************************
*
* FILE:     appsettings.cpp
*
* CREATED:  03-08-2010
*
* AUTHOR:   Benjamin Caspari (mail@becait.de)
*
* PURPOSE:  Sigleton class to store and load application wide settings
*
* Licensed under the GPL
*
***************************************************************************************************/
#include "appsettings.h"
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>


bool AppSettings::init()
{
	QCoreApplication::setOrganizationName(DEF_ORGANIZATION);
	QCoreApplication::setOrganizationDomain(DEF_ORGANIZATION_DOMAIN);
	QCoreApplication::setApplicationName(DEF_APPLICATION_NAME);

	getInstance().m_settingsDirectory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
	QDir dir(getInstance().m_settingsDirectory);
	if (!dir.mkpath(getInstance().m_settingsDirectory))
	{
		qCritical() << "Failed to create directory " 
			<< QDir::toNativeSeparators(getInstance().m_settingsDirectory);
		return false;
	}
	else
	{
		qDebug() << "Using config directory " 
			<< QDir::toNativeSeparators(getInstance().m_settingsDirectory);
	}

	return true;
}


AppSettings &AppSettings::getInstance()
{
    static AppSettings instance;
    return instance;
}


AppSettings::AppSettings()
{
}


const QString &AppSettings::settingsDirectory()
{
	return getInstance().m_settingsDirectory;
}