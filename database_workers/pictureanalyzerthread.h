#ifndef PICTUREANALYZERTHREAD_H
#define PICTUREANALYZERTHREAD_H
#include <QThread>
#include "../database/picturedatabase.h"

class PictureAnalyzerThread : public QThread
{
public:
    PictureAnalyzerThread();
    ~PictureAnalyzerThread();

    void startAnalyzer(const QString &file);

protected:
    virtual void run();

private:
    QString m_file;
    PictureDatabase *m_database;
};

#endif // PICTUREANALYZERTHREAD_H
