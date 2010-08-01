#ifndef DEBUG_H
#define DEBUG_H

#include <QString>
#include <QDebug>

class Debug
{
public:
    static void log(QString text)
    {
        qDebug() << text;
    }
};

#endif // DEBUG_H
