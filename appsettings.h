/***************************************************************************************************
*
* FILE:     appsettings.h
*
* CREATED:  03-08-2010
*
* AUTHOR:   Benjamin Caspari (becaspari@googlemail.com)
*
* PURPOSE:  Singleton class to store and load application wide settings
*
* Licensed under the GPL
*
***************************************************************************************************/

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>

class AppSettings : public QObject
{
Q_OBJECT
public:
    static AppSettings *getInstance();
    static QSettings *settings();

signals:

public slots:

private:
    explicit AppSettings(QObject *parent = 0);
    QSettings *m_settings;

};

#endif // APPSETTINGS_H
