#ifndef INDEXFILESTHREAD_H
#define INDEXFILESTHREAD_H

#include <QThread>
#include <QVector>
#include <QString>

#include "pictureinfo.h"

class IndexFilesThread : public QThread
{
    Q_OBJECT
public:
    explicit IndexFilesThread(QObject *parent = 0);
    void indexDirectory(QVector<PictureInfo*> *pictures,
                        QString directory,
                        bool subdirs);

signals:

public slots:
    void cancel();

protected:
    void run();

private:
    void addDirectory(QString directory, bool subdirs);
    void addFile(QString file);

    bool m_canceled;
    QString m_directory;
    QVector<PictureInfo*> *m_pictures;
    bool m_includeSubdirectories;
};

#endif // INDEXFILESTHREAD_H
