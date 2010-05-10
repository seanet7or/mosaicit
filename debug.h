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

#define log(text)   Debug::log(text)

#endif // DEBUG_H
